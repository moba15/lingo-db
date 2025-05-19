#include "lingodb/compiler/frontend/analyzer/analyzer/select_node_analyzer.h"

#include "lingodb/compiler/frontend/analyzer/analyzer/expression_analyzer.h"
#include "lingodb/runtime/RecordBatchInfo.h"

namespace lingodb::analyzer {
SelectNodeAnalyzer::SelectNodeAnalyzer(std::shared_ptr<catalog::Catalog> catalog) : catalog(std::move(catalog)) {
}

void SelectNodeAnalyzer::analyze(std::shared_ptr<ast::AstNode> rootNode, std::shared_ptr<SQLContext> context) {
   context->pushNewScope();
   if (rootNode->nodeType == ast::NodeType::QUERY_NODE) {
      auto queryNode = std::static_pointer_cast<ast::QueryNode>(rootNode);
      if (queryNode->type == ast::QueryNodeType::SELECT_NODE) {
         transform(rootNode, context);
         auto selectNode = std::static_pointer_cast<ast::SelectNode>(rootNode);

         auto selectList = selectNode->select_list;
         analyzeFromClause(selectNode, selectNode->from_clause, context);
         analyzeTargetSelections(selectNode, selectList, context);
         analyzeWhereClause(selectNode, selectNode->where_clause, context);
         analyzeModifiers(selectNode, selectNode->modifiers, context);
         if (selectNode->startPipeOperator) {
            //! TODO LOOP
            analyzePipeOperator(selectNode, selectNode->startPipeOperator, context);
         }
      } else if (queryNode->type == ast::QueryNodeType::PIPE_NODE) {
         auto pipeSelectNode = std::static_pointer_cast<ast::PipeSelectNode>(rootNode);
         if (pipeSelectNode->startPipeOperator->node->nodeType != ast::NodeType::TABLE_REF) {
            if (pipeSelectNode->startPipeOperator->node->nodeType == ast::NodeType::QUERY_NODE) {
               auto startQueryNode = std::static_pointer_cast<ast::QueryNode>(pipeSelectNode->startPipeOperator->node);
               analyze(startQueryNode, context);

            } else {
               throw std::runtime_error("Should not happen");
            }

         } else {
            analyzeFromClause(queryNode, std::static_pointer_cast<ast::TableRef>(pipeSelectNode->startPipeOperator->node), context);
         }
         auto currentPipeOp = pipeSelectNode->startPipeOperator->next;
         while (currentPipeOp) {
            analyzePipeOperator(pipeSelectNode, currentPipeOp, context);
            currentPipeOp = currentPipeOp->next;
         }
      }
   }
}

void SelectNodeAnalyzer::transform(std::shared_ptr<ast::AstNode> rootNode, std::shared_ptr<SQLContext> context) {
   auto selectNode = std::static_pointer_cast<ast::SelectNode>(rootNode);
   auto selectList = selectNode->select_list;

   /**
    * analyzeTargetSelections and analyzeGroupByClause
    * and extract aggregations
    */
   transformTargetSelections(selectList, context);
   transformGroupByClause(selectNode, selectNode->groups, context);
   if (aggregation && (!aggregation->aggregations.empty() || aggregation->groupByNode)) {
      auto pipeOperator = drv.nf.node<ast::PipeOperator>(aggregation->loc, lingodb::ast::PipeOperatorType::AGGREGATE, aggregation);

      selectNode->startPipeOperator = pipeOperator;
      if (selectNode->endPipeOperator) {
         selectNode->endPipeOperator->next = pipeOperator;
      }
      if (selectNode->startPipeOperator) {
         selectNode->startPipeOperator = pipeOperator;
      }
      selectNode->endPipeOperator = pipeOperator;

      if (selectNode->having) {
         auto havingPipeOp = drv.nf.node<ast::PipeOperator>(selectNode->having->loc, ast::PipeOperatorType::WHERE, selectNode->having);
         //TODO check
         selectNode->having.reset();
         selectNode->endPipeOperator->next = havingPipeOp;
         selectNode->endPipeOperator = havingPipeOp;
      }
   }
}

void SelectNodeAnalyzer::transformTargetSelections(std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context) {
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
      }
      ++it;
   }

   std::transform(aggregationExpressions.begin(), aggregationExpressions.end(),
                  std::back_inserter(aggregation->aggregations),
                  [](const auto& expr) {
                     return std::static_pointer_cast<ast::FunctionExpression>(expr);
                  });
}
void SelectNodeAnalyzer::transformGroupByClause(std::shared_ptr<ast::SelectNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context) {
   if (groupByNode) {
      aggregation->groupByNode = std::move(groupByNode);
      rootSelectNode->groups = nullptr;
   }
}

void SelectNodeAnalyzer::analyzeFromClause(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::TableRef> tableRef, std::shared_ptr<SQLContext> context) {
   if (!tableRef) {
      return;
   }
   if (tableRef->type == ast::TableReferenceType::BASE_TABLE) {
      auto baseTableRef = std::static_pointer_cast<ast::BaseTableRef>(tableRef);
      auto catalogEntry = catalog->getTypedEntry<catalog::TableCatalogEntry>(baseTableRef->tableName);
      if (!catalogEntry.has_value()) {
         error("No Catalog found with name " + baseTableRef->tableName, baseTableRef->loc);
         return;
      }
      baseTableRef->catalogEntry = catalogEntry.value();
      auto tableName = baseTableRef->alias.empty() ? baseTableRef->tableName : baseTableRef->alias;
      context->currentScope->tables.emplace(std::pair{tableName, catalogEntry.value()});
   } else if (tableRef->type == ast::TableReferenceType::JOIN) {
      auto join = std::static_pointer_cast<ast::JoinRef>(tableRef);
      if (std::holds_alternative<std::shared_ptr<ast::TableRef>>(join->left)) {
         analyzeFromClause(rootNode, std::get<std::shared_ptr<ast::TableRef>>(join->left), context);
      } else {
         auto queryNode = std::get<std::shared_ptr<ast::QueryNode>>(join->left);
         if (queryNode->type == ast::QueryNodeType::PIPE_NODE) {
            auto pipeNode = std::static_pointer_cast<ast::PipeSelectNode>(queryNode);
            analyze(pipeNode, context);
         } else {
            throw std::runtime_error("Not implemented");
         }
      }

      analyzeFromClause(rootNode, join->right, context);
      ExpressionAnalyzer exprAnalyzer{};
      if (std::holds_alternative<std::shared_ptr<ast::ParsedExpression>>(join->condition)) {
         auto cond = std::get<std::shared_ptr<ast::ParsedExpression>>(join->condition);
         if (cond) {
            exprAnalyzer.analyze(cond, context);
         } else {
            if (join->refType != ast::JoinCondType::CROSS) {
               error("Join condition is empty", join->loc);
            }
         }

      } else if (std::holds_alternative<std::vector<std::shared_ptr<ast::ColumnRefExpression>>>(join->condition)) {
         for (auto& col : std::get<std::vector<std::shared_ptr<ast::ColumnRefExpression>>>(join->condition)) {
            exprAnalyzer.analyze(col, context);
         }
      }

   } else if (tableRef->type == ast::TableReferenceType::SUBQUERY) {
      auto subQueryRef = std::static_pointer_cast<ast::SubqueryRef>(tableRef);
      if (subQueryRef->alias.empty()) {
         error("Alias for subquery must not be empty", subQueryRef->loc);
         return;
      }
      analyze(subQueryRef->subSelectNode, context);

   }

   else {
      throw std::runtime_error("Not implemented4");
   }
}

template <class T>
void SelectNodeAnalyzer::analyzeTargetSelections(std::shared_ptr<T> rootPipeSelectNode, std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context) {
   ExpressionAnalyzer expressionAnalyzer{};
   rootPipeSelectNode->targetInfo.namedResults.clear();
   if (targetSelection) {
      for (auto& target : targetSelection->targets) {
         expressionAnalyzer.analyze(target, context);
         if (target->type == ast::ExpressionType::COLUMN_REF) {
            auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(target);
            for (auto column : columnRef->columns) {
               targetSelection->targetColumns.emplace_back(std::pair<std::string, catalog::Column&>(column.getColumnName(), column));
               auto name = target->alias.empty() ? column.getColumnName() : target->alias;
               rootPipeSelectNode->targetInfo.map(name, std::make_shared<ast::ColumnInfo>(columnRef->scope, column));
            }
            if (columnRef->refsAggregationFunction) {
               rootPipeSelectNode->targetInfo.map(columnRef->fName, context->currentScope->functionsEntry.at(columnRef->fName));
            }
         } else if (target->type == ast::ExpressionType::STAR) {
            auto star = std::static_pointer_cast<ast::StarExpression>(target);
            for (auto [scope, column] : star->columns) {
               targetSelection->targetColumns.emplace_back(std::pair<std::string, catalog::Column&>(column.getColumnName(), column));
               auto name = target->alias.empty() ? column.getColumnName() : target->alias;
               //TODO check for correct scope
               rootPipeSelectNode->targetInfo.map(name, std::make_shared<ast::ColumnInfo>(scope, column));
            }
         } else if (target->type == ast::ExpressionType::AGGREGATE && target->exprClass == ast::ExpressionClass::FUNCTION) {
            //Aggregation function
            auto function = std::static_pointer_cast<ast::FunctionExpression>(target);
            auto fName = function->alias.empty() ? function->functionName : function->alias;
            rootPipeSelectNode->targetInfo.map(fName, context->currentScope->functionsEntry.at(fName));
         } else {
            error("Currently this Targetselection is not implemented", target->loc);
         }
      }
   }
}

void SelectNodeAnalyzer::analyzeWhereClause(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::ParsedExpression> whereClause, std::shared_ptr<SQLContext> context) {
   ExpressionAnalyzer exprAnalyzer{};
   if (whereClause) {
      exprAnalyzer.analyze(whereClause, context);
   }
}

void SelectNodeAnalyzer::analyzeAggregation(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::AggregationNode> aggregationNode, std::shared_ptr<SQLContext> context) {
   if (aggregationNode->groupByNode) {
      analyzeGroupByNode(rootSelectNode, aggregationNode->groupByNode, context);
   }
   ExpressionAnalyzer exprAnalyzer{};
   for (auto aggregation : aggregationNode->aggregations) {
      exprAnalyzer.analyze(aggregation, context);
   }
}

void SelectNodeAnalyzer::analyzeGroupByNode(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context) {
   ExpressionAnalyzer exprAnalyzer{};
   for (auto expr : groupByNode->group_expressions) {
      exprAnalyzer.analyze(expr, context);
   }
}

void SelectNodeAnalyzer::analyzeModifiers(std::shared_ptr<ast::SelectNode> rootNode, std::vector<std::shared_ptr<ast::ResultModifier>> modifiers, std::shared_ptr<SQLContext> context) {
   for (auto modifier : modifiers) {
      if (modifier->modifierType == ast::ResultModifierType::ORDER_BY) {
         auto orderModifier = std::static_pointer_cast<ast::OrderByModifier>(modifier);
         analyzeOrderByModifier(rootNode, orderModifier, context);
      } else if (modifier->modifierType == ast::ResultModifierType::LIMIT) {
         auto limitModifier = std::static_pointer_cast<ast::LimitModifier>(modifier);
         analyzeLimitModifier(rootNode, limitModifier, context);
      } else {
         throw std::runtime_error("Modifier not implemented");
      }
   }
}
void SelectNodeAnalyzer::analyzeOrderByModifier(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::OrderByModifier> orderByModifier, std::shared_ptr<SQLContext> context) {
   ExpressionAnalyzer exprAnalyzer{};
   for (auto orderByElements : orderByModifier->orderByElements) {
      if (orderByElements->expression) {
         exprAnalyzer.analyze(orderByElements->expression, context);
      }
   }
}

void SelectNodeAnalyzer::analyzeLimitModifier(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::LimitModifier> limitModifier, std::shared_ptr<SQLContext> context) {
   ExpressionAnalyzer exprAnalyzer{};
   if (!limitModifier->limitExpression) {
      throw std::runtime_error("Limit expression is empty");
   }
   exprAnalyzer.analyze(limitModifier->limitExpression, context);
   if (limitModifier->limitExpression->exprClass != ast::ExpressionClass::CONSTANT && limitModifier->limitExpression->type != ast::ExpressionType::VALUE_CONSTANT) {
      throw std::runtime_error("Limit expression is not a constant");
   }
   auto constantExpression = std::static_pointer_cast<ast::ConstantExpression>(limitModifier->limitExpression);
   if (constantExpression->value->type != ast::ConstantType::INT) {
      throw std::runtime_error("Limit expression is not an integer");
   }
   auto intValue = std::static_pointer_cast<ast::IntConstantValue>(constantExpression->value);
   if (intValue < 0) {
      throw std::runtime_error("Limit expression is negative");
   }
}

template <class T>
void SelectNodeAnalyzer::analyzePipeOperator(std::shared_ptr<T> rootNode, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<SQLContext> context) {
   ExpressionAnalyzer exprAnalyzer{};
   if (pipeOperator->type == ast::PipeOperatorType::SELECT) {
      assert(pipeOperator->node->nodeType == ast::NodeType::EXPRESSION);
      auto expr = std::static_pointer_cast<ast::ParsedExpression>(pipeOperator->node);
      auto targetSelection = std::static_pointer_cast<ast::TargetsExpression>(pipeOperator->node);
      //TODO also call exprAnalyzer
      analyzeTargetSelections(rootNode, targetSelection, context);

   } else if (pipeOperator->type == ast::PipeOperatorType::AGGREGATE) {
      assert(pipeOperator->node->nodeType == ast::NodeType::AGGREGATION);
      auto aggregation = std::static_pointer_cast<ast::AggregationNode>(pipeOperator->node);
      analyzeAggregation(rootNode, aggregation, context);

   } else if (pipeOperator->type == ast::PipeOperatorType::ORDER_BY) {
      auto orderBy = std::static_pointer_cast<ast::OrderByModifier>(pipeOperator->node);
      analyzeOrderByModifier(rootNode, orderBy, context);
   } else {
      auto nodeType = pipeOperator->node->nodeType;
      if (nodeType == ast::NodeType::EXPRESSION) {
         auto expr = std::static_pointer_cast<ast::ParsedExpression>(pipeOperator->node);
         assert(expr->type != ast::ExpressionType::TARGETS);

         exprAnalyzer.analyze(expr, context);

      } else if (nodeType == ast::NodeType::TABLE_REF) {
         throw std::runtime_error("Not implemented2");
      } else {
         throw std::runtime_error("Not implemented");
      }
   }
}
} // namespace lingodb::analyzer