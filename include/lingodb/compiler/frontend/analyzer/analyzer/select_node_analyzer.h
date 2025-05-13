#pragma once

#include "lingodb/compiler/frontend/analyzer/analyzer.h"
#include "lingodb/compiler/frontend/analyzer/sql_context.h"
#include "lingodb/compiler/frontend/sql-parser/aggregation_node.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/targets_expression.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
#include "lingodb/compiler/frontend/sql-parser/query_node/select_node.h"

namespace lingodb::analyzer {
class SelectNodeAnalyzer : public Analyzer {
   public:
   explicit SelectNodeAnalyzer(std::shared_ptr<catalog::Catalog> catalog);

   void analyze(std::shared_ptr<ast::AstNode> rootNode, std::shared_ptr<SQLContext> context) override;
   void transform(std::shared_ptr<ast::AstNode> rootNode, std::shared_ptr<SQLContext> context);

   private:
   std::shared_ptr<catalog::Catalog> catalog;
   std::shared_ptr<ast::AggregationNode> aggregation = std::make_shared<ast::AggregationNode>();

   void transformTargetSelections(std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context);
   void transformGroupByClause(std::shared_ptr<ast::SelectNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context);

   void analyzeTargetSelections(std::shared_ptr<ast::SelectNode> rootSelectNode, std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context);
   void analyzeTargetSelections(std::shared_ptr<ast::PipeSelectNode> rootPipeSelectNode, std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context);
   void analyzeFromClause(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::TableRef> tableRef, std::shared_ptr<SQLContext> context);
   void analyzeWhereClause(std::shared_ptr<ast::SelectNode> rootSelectNode, std::shared_ptr<ast::ParsedExpression> whereClause, std::shared_ptr<SQLContext> context);
   void analyzeGroupByClause(std::shared_ptr<ast::SelectNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context);

   void analyzeOrderByClause(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<SQLContext> context);

   void error(std::string message, location loc) {
      std::ostringstream s{};
      s << message << " at " << loc << std::endl;
      throw std::runtime_error(s.str());
   }
};
} // namespace lingodb::analyzer