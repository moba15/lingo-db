#include "lingodb/compiler/frontend/analyzer/analyzer/select_node_analyzer.h"

#include "lingodb/compiler/frontend/sql-parser/query_node/pipe_select_node.h"
#include "lingodb/compiler/frontend/sql-parser/query_node/select_node.h"

namespace lingodb::analyzer {
void SelectNodeAnalyzer::analyze(std::shared_ptr<ast::AstNode> rootNode, std::shared_ptr<SQLContext> context) {
   auto selectNode = std::static_pointer_cast<ast::SelectNode>(rootNode);
   auto selectList = selectNode->select_list;
   /**
    * analyzeTargetSelections and analyzeGroupByClause
    * and extract aggregations
    */
   analyzeTargetSelections(selectList, context);
   analyzeGroupByClause(selectNode, selectNode->groups, context);

   auto pipeOperator = drv.nf.node<ast::PipeOperator>(aggregation->loc, aggregation);

   selectNode->startPipeOperator = pipeOperator;
   if (selectNode->endPipeOperator) {
      selectNode->endPipeOperator->next = pipeOperator;
   }
   if (selectNode->startPipeOperator) {
      selectNode->startPipeOperator = pipeOperator;
   }
   selectNode->endPipeOperator = pipeOperator;

   if (selectNode->having) {
      auto havingPipeOp = drv.nf.node<ast::PipeOperator>(selectNode->having->loc, selectNode->having);
      //TODO check
      selectNode->having.reset();
      selectNode->endPipeOperator->next = havingPipeOp;
      selectNode->endPipeOperator = havingPipeOp;
   }
}
void SelectNodeAnalyzer::analyzeTargetSelections(std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context) {
   if (!targetSelection || targetSelection->targets.empty())
      error("Empty target selection", targetSelection->loc);
   /**
    * Extract all target expression which are aggregation functions
    * To be able to to put the in the AggregationNode togther with Group by
    */
   std::vector<std::shared_ptr<ast::ParsedExpression>> aggregationExpressions;
   auto it = targetSelection->targets.begin();
   while (it != targetSelection->targets.end()) {
      if ((*it)->type == ast::ExpressionType::AGGREGATE && (*it)->exprClass == ast::ExpressionClass::FUNCTION) {
         aggregation->aggregations.emplace_back(std::static_pointer_cast<ast::FunctionExpression>(*it));
         it = targetSelection->targets.erase(it);
      } else {
         ++it;
      }
   }

   std::transform(aggregationExpressions.begin(), aggregationExpressions.end(),
                  std::back_inserter(aggregation->aggregations),
                  [](const auto& expr) {
                     return std::static_pointer_cast<ast::FunctionExpression>(expr);
                  });
}
void SelectNodeAnalyzer::analyzeFromClause(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<SQLContext> context) {
   //TODO
}
void SelectNodeAnalyzer::analyzeWhereClause(std::shared_ptr<ast::SelectNode> rootSelectNode, std::vector<std::shared_ptr<ast::ParsedExpression>> whereClause, std::shared_ptr<SQLContext> context) {
   /**
    * Put into pipe sql similiar structure
    * //TODO Handle subquerys
    */
}

void SelectNodeAnalyzer::analyzeGroupByClause(std::shared_ptr<ast::SelectNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context) {
   if (groupByNode) {
      aggregation->groupByNode = std::move(groupByNode);
      rootSelectNode->groups = nullptr;
   }
}
void SelectNodeAnalyzer::analyzeOrderByClause(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<SQLContext> context) {
   //TODO
}
} // namespace lingodb::analyzer