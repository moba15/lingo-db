#include "lingodb/compiler/frontend/analyzer/sql_analyzer.h"
namespace lingodb::analyzer {
SQLQueryAnalyzer::SQLQueryAnalyzer(std::shared_ptr<catalog::Catalog> catalog) : catalog(std::move(catalog)) {
}
std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyze(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context) {
   context->pushNewScope();
   //First Transform

   return transform(rootNode, std::make_shared<ASTTransformContext>());

   /*if (rootNode->nodeType == ast::NodeType::QUERY_NODE) {
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
   }*/
}

std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::transform(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<ASTTransformContext> context) {
   switch (rootNode->nodeType) {
      case ast::NodeType::QUERY_NODE: {
         auto queryNode = std::static_pointer_cast<ast::QueryNode>(rootNode);
         switch (queryNode->type) {
            case ast::QueryNodeType::SELECT_NODE: {
               auto selectNode = std::static_pointer_cast<ast::SelectNode>(queryNode);
               std::shared_ptr<ast::TableProducer> transformed = nullptr;
               //Transform from_clause
               if (selectNode->from_clause) {
                  auto transformedFrom = transformCast<ast::TableRef>(selectNode->from_clause, context);

                  selectNode->from_clause = nullptr;
                  transformed = transformedFrom;
               }

               //Transform where_clause
               if (selectNode->where_clause) {
                  //TODO maybe without Pipeoperator??
                  auto pipe = drv.nf.node<ast::PipeOperator>(selectNode->where_clause->loc, ast::PipeOperatorType::WHERE, selectNode->where_clause);
                  auto transFormededWhereClause = transformCast<ast::PipeOperator>(pipe, context);
                  transFormededWhereClause->input = transformed;
                  selectNode->where_clause = nullptr;
                  transformed = transFormededWhereClause;
               }

               //Transform modifiers
               for (auto modifier : selectNode->modifiers) {
                  auto transformedModifier = transformCast<ast::PipeOperator>(modifier, context);
                  transformedModifier->input = transformed;
                  transformed = transformedModifier;
               }
               selectNode->modifiers.clear();

               //Transform Group by
               if (selectNode->groups) {
                  auto loc = selectNode->groups->loc;
                  context->aggregationNode->groupByNode = std::move(selectNode->groups);
                  auto aggPipeNode = drv.nf.node<ast::PipeOperator>(loc, ast::PipeOperatorType::AGGREGATE, context->aggregationNode);
                  auto transFormedAggregation = transformCast<ast::PipeOperator>(aggPipeNode, context);
                  transFormedAggregation->input = transformed;
                  transformed = transFormedAggregation;
               }

               //Transform target selection
               auto select_list = selectNode->select_list;
               if (select_list) {
                  auto pipe = drv.nf.node<ast::PipeOperator>(select_list->loc, ast::PipeOperatorType::SELECT, select_list);
                  auto transformedSelect = transformCast<ast::PipeOperator>(pipe, context);
                  transformedSelect->input = transformed;
                  transformed = transformedSelect;
                  selectNode->select_list = nullptr;
               }

               return transformed;
            }
            default: return queryNode;
         }
      }
      case ast::NodeType::PIPE_OP: {
         auto pipeOp = std::static_pointer_cast<ast::PipeOperator>(rootNode);
         if (pipeOp->input) {
           pipeOp->input = transform(pipeOp->input, context);
         }

         switch (pipeOp->pipeOpType) {
            case ast::PipeOperatorType::SELECT: {
               auto selectNode = std::static_pointer_cast<ast::TargetsExpression>(pipeOp->node);
               //Extract AggFunctions
               for (auto target : selectNode->targets) {
                  if (target->type == ast::ExpressionType::AGGREGATE && target->exprClass == ast::ExpressionClass::FUNCTION) {
                     context->aggregationNode->aggregations.push_back(std::static_pointer_cast<ast::FunctionExpression>(target));
                  }
               }

               return pipeOp;
            }
            default: return pipeOp;
         }

         return pipeOp;
      }
      default:
         return rootNode;
   }
   return rootNode;
}

template <class T>
std::shared_ptr<T> SQLQueryAnalyzer::transformCast(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<ASTTransformContext> context) {

   return std::static_pointer_cast<T>(transform(rootNode, context));
}

void SQLQueryAnalyzer::transformTargetSelections(std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context) {
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
         //aggregation->aggregations.emplace_back(std::static_pointer_cast<ast::FunctionExpression>(*it));
      }
      ++it;
   }

   /*std::transform(aggregationExpressions.begin(), aggregationExpressions.end(),
                  std::back_inserter(aggregation->aggregations),
                  [](const auto& expr) {
                     return std::static_pointer_cast<ast::FunctionExpression>(expr);
                  });*/
}
void SQLQueryAnalyzer::transformGroupByClause(std::shared_ptr<ast::SelectNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context) {
   if (groupByNode) {
      //aggregation->groupByNode = std::move(groupByNode);
      rootSelectNode->groups = nullptr;
   }
}

void SQLQueryAnalyzer::analyzeFromClause(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::TableRef> tableRef, std::shared_ptr<SQLContext> context) {
   /*if (!tableRef) {
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

      if (std::holds_alternative<std::shared_ptr<ast::ParsedExpression>>(join->condition)) {
         auto cond = std::get<std::shared_ptr<ast::ParsedExpression>>(join->condition);
         if (cond) {
            analyzeExpression(cond, context);
         } else {
            if (join->refType != ast::JoinCondType::CROSS) {
               error("Join condition is empty", join->loc);
            }
         }

      } else if (std::holds_alternative<std::vector<std::shared_ptr<ast::ColumnRefExpression>>>(join->condition)) {
         for (auto& col : std::get<std::vector<std::shared_ptr<ast::ColumnRefExpression>>>(join->condition)) {
            analyzeExpression(col, context);
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
   }*/
}

template <class T>
void SQLQueryAnalyzer::analyzeTargetSelections(std::shared_ptr<T> rootPipeSelectNode, std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context) {
   rootPipeSelectNode->targetInfo.namedResults.clear();
   if (targetSelection) {
      for (auto& target : targetSelection->targets) {
         analyzeExpression(target, context);
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

void SQLQueryAnalyzer::analyzeWhereClause(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::ParsedExpression> whereClause, std::shared_ptr<SQLContext> context) {
   if (whereClause) {
      analyzeExpression(whereClause, context);
   }
}

void SQLQueryAnalyzer::analyzeAggregation(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::AggregationNode> aggregationNode, std::shared_ptr<SQLContext> context) {
   if (aggregationNode->groupByNode) {
      analyzeGroupByNode(rootSelectNode, aggregationNode->groupByNode, context);
   }
   for (auto aggregation : aggregationNode->aggregations) {
      analyzeExpression(aggregation, context);
   }
}

void SQLQueryAnalyzer::analyzeGroupByNode(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context) {
   for (auto expr : groupByNode->group_expressions) {
      analyzeExpression(expr, context);
   }
}

void SQLQueryAnalyzer::analyzeModifiers(std::shared_ptr<ast::SelectNode> rootNode, std::vector<std::shared_ptr<ast::ResultModifier>> modifiers, std::shared_ptr<SQLContext> context) {
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
void SQLQueryAnalyzer::analyzeOrderByModifier(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::OrderByModifier> orderByModifier, std::shared_ptr<SQLContext> context) {
   for (auto orderByElements : orderByModifier->orderByElements) {
      if (orderByElements->expression) {
         analyzeExpression(orderByElements->expression, context);
      }
   }
}

void SQLQueryAnalyzer::analyzeLimitModifier(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::LimitModifier> limitModifier, std::shared_ptr<SQLContext> context) {
   if (!limitModifier->limitExpression) {
      throw std::runtime_error("Limit expression is empty");
   }
   analyzeExpression(limitModifier->limitExpression, context);
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
void SQLQueryAnalyzer::analyzePipeOperator(std::shared_ptr<T> rootNode, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<SQLContext> context) {
   if (pipeOperator->pipeOpType == ast::PipeOperatorType::SELECT) {
      assert(pipeOperator->node->nodeType == ast::NodeType::EXPRESSION);
      auto expr = std::static_pointer_cast<ast::ParsedExpression>(pipeOperator->node);
      auto targetSelection = std::static_pointer_cast<ast::TargetsExpression>(pipeOperator->node);
      //TODO also call exprAnalyzer
      analyzeTargetSelections(rootNode, targetSelection, context);

   } else if (pipeOperator->pipeOpType == ast::PipeOperatorType::AGGREGATE) {
      assert(pipeOperator->node->nodeType == ast::NodeType::AGGREGATION);
      auto aggregation = std::static_pointer_cast<ast::AggregationNode>(pipeOperator->node);
      analyzeAggregation(rootNode, aggregation, context);

   } else if (pipeOperator->pipeOpType == ast::PipeOperatorType::ORDER_BY) {
      auto orderBy = std::static_pointer_cast<ast::OrderByModifier>(pipeOperator->node);
      analyzeOrderByModifier(rootNode, orderBy, context);
   } else {
      auto nodeType = pipeOperator->node->nodeType;
      if (nodeType == ast::NodeType::EXPRESSION) {
         auto expr = std::static_pointer_cast<ast::ParsedExpression>(pipeOperator->node);
         assert(expr->type != ast::ExpressionType::TARGETS);

         analyzeExpression(expr, context);

      } else if (nodeType == ast::NodeType::TABLE_REF) {
         throw std::runtime_error("Not implemented2");
      } else {
         throw std::runtime_error("Not implemented");
      }
   }
}

///Expressions

void SQLQueryAnalyzer::analyzeExpression(std::shared_ptr<ast::ParsedExpression> rootNode, std::shared_ptr<SQLContext> context) {
   if (rootNode->nodeType == ast::NodeType::EXPRESSION) {
      auto expr = std::static_pointer_cast<ast::ParsedExpression>(rootNode);
      if (expr->exprClass == ast::ExpressionClass::COMPARISON) {
         auto comp = std::static_pointer_cast<ast::ComparisonExpression>(expr);
         analyzeComparisonExpression(comp, context);
      } else if (expr->exprClass == ast::ExpressionClass::CONSTANT) {
         auto constExpr = std::static_pointer_cast<ast::ConstantExpression>(expr);
         analyzeConstExpression(constExpr, context);
      } else if (expr->exprClass == ast::ExpressionClass::COLUMN_REF) {
         auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(expr);
         analyzeColumnRefExpression(columnRef, context);
      } else if (expr->exprClass == ast::ExpressionClass::CONJUNCTION) {
         auto conjunction = std::static_pointer_cast<ast::ConjunctionExpression>(expr);
         analyzeConjunctionExpression(conjunction, context);
      } else if (expr->exprClass == ast::ExpressionClass::STAR) {
         auto star = std::static_pointer_cast<ast::StarExpression>(expr);
         analyzeStarRefExpression(star, context);
      } else if (expr->exprClass == ast::ExpressionClass::FUNCTION) {
         if (expr->type == ast::ExpressionType::AGGREGATE) {
            analyzeAggregationFunctionExpression(std::static_pointer_cast<ast::FunctionExpression>(expr), context);

         } else {
            throw std::runtime_error("Not implemented");
         }
      } else {
         throw std::runtime_error("Not implemented");
      }
   }
}

void SQLQueryAnalyzer::analyzeComparisonExpression(std::shared_ptr<ast::ComparisonExpression> comparison, std::shared_ptr<SQLContext> context) {
   analyzeExpression(comparison->left, context);
   analyzeExpression(comparison->right, context);
   if (!comparison->left->resultType.has_value()) {
      error("Left side of comparison is not a valid expression", comparison->left->loc);
      return;
   }
   if (!comparison->right->resultType.has_value()) {
      error("Right side of comparison is not a valid expression", comparison->right->loc);
      return;
   }
   if (comparison->left->resultType.value().getTypeId() != comparison->right->resultType.value().getTypeId()) {
      error("Comparison is not possible between " + comparison->left->resultType.value().toString() + " and " + comparison->right->resultType.value().toString(), comparison->loc);
      return;
   }
   comparison->resultType = catalog::Type::boolean();
}

void SQLQueryAnalyzer::analyzeColumnRefExpression(std::shared_ptr<ast::ColumnRefExpression> columnRef, std::shared_ptr<SQLContext> context) {
   auto columnName = columnRef->column_names.size() == 1 ? columnRef->column_names[0] : columnRef->column_names[1];
   std::vector<catalog::Column> columns;
   std::string scope;
   if (columnRef->column_names.size() == 2) {
      auto foundColumn = context->findColumn(columnRef->column_names[1], columnRef->column_names[0]);
      columns = foundColumn.second;
      scope = foundColumn.first;
   } else if (columnRef->column_names.size() == 1) {
      auto foundColumn = context->findColumn(columnRef->column_names[0]);
      columns = foundColumn.second;
      scope = foundColumn.first;
   } else {
      throw std::runtime_error("Not implemented");
   }

   if (columns.empty()) {
      //TODO check function
      auto function = context->findFunction(columnRef->column_names[0]);
      if (function.second) {
         columnRef->scope = function.second->scope;
         //TODO type!
         columnRef->resultType = catalog::Type::int64();
         columnRef->refsAggregationFunction = true;
         columnRef->fName = function.second->name;
         return;
      }
      if (columnRef->column_names.size() == 1)
         error("No column found with name " + columnName, columnRef->loc);
      else
         error("No column found with name " + columnRef->column_names[0] + "." + columnName, columnRef->loc);
      return;
   }
   if (columns.size() > 1) {
      error(columnName + " is ambiguous", columnRef->loc);
      return;
   }
   columnRef->columns = columns;
   columnRef->resultType = columns.at(0).getLogicalType();
   columnRef->scope = std::move(scope);
}

void SQLQueryAnalyzer::analyzeStarRefExpression(const std::shared_ptr<ast::StarExpression> star, const std::shared_ptr<SQLContext> context) {
   std::vector<catalog::Catalog> catalogs;
   std::string scope;
   if (star->relationName.empty()) {
      star->columns = std::move(context->getColumns());
   } else {
      star->columns = std::move(context->getColumns(star->relationName));
   }
}

void SQLQueryAnalyzer::analyzeConstExpression(std::shared_ptr<ast::ConstantExpression> constExpr, std::shared_ptr<SQLContext> context) {
   //TODO set correct type
   if (!constExpr->value) {
      throw std::runtime_error("Value of constExpr is empty");
   }
   switch (constExpr->value->type) {
      case ast::ConstantType::INT:
         constExpr->resultType = catalog::Type::int64();
         break;
      case ast::ConstantType::STRING:
         constExpr->resultType = catalog::Type::stringType();
         break;
      default:
         throw std::runtime_error("Not implemented");
   }
}

void SQLQueryAnalyzer::analyzeConjunctionExpression(std::shared_ptr<ast::ConjunctionExpression> conjunction, std::shared_ptr<SQLContext> context) {
   for (auto expr : conjunction->children) {
      analyzeExpression(expr, context);
      if (!expr->resultType.has_value() || expr->resultType.value().getTypeId() != catalog::Type::boolean().getTypeId()) {
         error("Conjunction is not possible with children of type boolean", expr->loc);
         return;
      }
   }
   conjunction->resultType = catalog::Type::boolean();
}

void SQLQueryAnalyzer::analyzeAggregationFunctionExpression(std::shared_ptr<ast::FunctionExpression> function, std::shared_ptr<SQLContext> context) {
   //TODO Better
   if (!function->scope.empty()) {
      return;
   }
   for (auto arg : function->arguments) {
      analyzeExpression(arg, context);
   }
   function->scope = createTmpScope();
   auto fName = function->alias.empty() ? function->functionName : function->alias;
   context->currentScope->functionsEntry.emplace(fName, std::make_shared<ast::FunctionInfo>(function->scope, fName));
}

} // namespace lingodb::analyzer