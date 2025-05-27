#include "lingodb/compiler/frontend/analyzer/sql_analyzer.h"

#include "lingodb/compiler/frontend/SQL/Parser.h"
#include "lingodb/compiler/frontend/analyzer/bound/bound_aggregation.h"
#include "lingodb/compiler/frontend/analyzer/bound/bound_groupby.h"
#include "lingodb/compiler/frontend/analyzer/bound/bound_tableref.h"

#include <functional>
#include <ranges>
namespace lingodb::analyzer {
SQLQueryAnalyzer::SQLQueryAnalyzer(std::shared_ptr<catalog::Catalog> catalog) : catalog(std::move(catalog)) {
}
std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyzeAndTransform(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context) {
   auto transformed = transform(rootNode, std::make_shared<ASTTransformContext>());
   context->pushNewScope();
   analyze(transformed, context);
   return transformed;
}
std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyze(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context) {
   //First Transform
   switch (rootNode->nodeType) {
      case ast::NodeType::PIPE_OP: {
         auto pipeOp = std::static_pointer_cast<ast::PipeOperator>(rootNode);
         if (pipeOp->input) {
            pipeOp->input = analyze(pipeOp->input, context);
         }
         return analyzePipeOperator(pipeOp, context);
      }
      case ast::NodeType::TABLE_REF: {
         auto tableRef = std::static_pointer_cast<ast::TableRef>(rootNode);
         return analyzeTableRef(tableRef, context);
      }
      case ast::NodeType::RESULT_MODIFIER: {
         auto resultModifier = std::static_pointer_cast<ast::ResultModifier>(rootNode);
         if (resultModifier->input) {
            resultModifier->input = analyze(resultModifier->input, context);
         }
         return analyzeResultModifier(resultModifier, context);
      }
      default: throw std::runtime_error("Not implemented");
   }
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
                  auto pipe = drv.nf.node<ast::PipeOperator>(selectNode->where_clause->loc, ast::PipeOperatorType::WHERE, selectNode->where_clause);
                  auto transFormededWhereClause = transformCast<ast::PipeOperator>(pipe, context);
                  transFormededWhereClause->input = transformed;
                  selectNode->where_clause = nullptr;
                  transformed = transFormededWhereClause;
               }

               //Transform modifiers
               for (auto modifier : selectNode->modifiers) {
                  auto transformedModifier = transformCast<ast::ResultModifier>(modifier, context);
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
      case ast::NodeType::TABLE_REF: {
         auto tableRef = std::static_pointer_cast<ast::TableRef>(rootNode);
         switch (tableRef->type) {
            case ast::TableReferenceType::BASE_TABLE: {
               return tableRef;
            }
            case ast::TableReferenceType::JOIN: {
               auto joinRef = std::static_pointer_cast<ast::JoinRef>(tableRef);
               if (joinRef->left) {
                  joinRef->left = transform(joinRef->left, context);
               }
               if (joinRef->right) {
                  joinRef->right = transform(joinRef->right, context);
               }

               return tableRef;
            }
            case ast::TableReferenceType::SUBQUERY: {
               auto subquery = std::static_pointer_cast<ast::SubqueryRef>(tableRef);
               auto transformedSubSelectNode = transform(subquery->subSelectNode, context);
               subquery->subSelectNode = transformedSubSelectNode;
               return subquery;
            }
            default: return tableRef;
         }
      }
      default:
         return rootNode;
   }
}

template <class T>
std::shared_ptr<T> SQLQueryAnalyzer::transformCast(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<ASTTransformContext> context) {
   return std::static_pointer_cast<T>(transform(rootNode, context));
}

std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyzePipeOperator(std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<SQLContext> context) {
   std::shared_ptr<ast::AstNode> boundAstNode = pipeOperator->node;
   switch (pipeOperator->pipeOpType) {
      case ast::PipeOperatorType::SELECT: {
         assert(pipeOperator->node->nodeType == ast::NodeType::EXPRESSION);
         auto targetSelection = std::static_pointer_cast<ast::TargetsExpression>(pipeOperator->node);
         std::vector<std::shared_ptr<ast::BoundExpression>> boundTargetExpressions{};
         std::vector<std::pair<std::string, catalog::Column>> targetColumns{};
         for (auto& target : targetSelection->targets) {
            auto parsedExpression = analyzeExpression(target, context);
            switch (parsedExpression->exprClass) {
               case ast::ExpressionClass::BOUND_COLUMN_REF: {
                  //ADD column_ref to targetInfo for the current scope!
                  auto columnRef = std::static_pointer_cast<ast::BoundColumnRefExpression>(parsedExpression);
                  auto column = columnRef->boundColumn;
                  targetColumns.emplace_back(std::pair<std::string, catalog::Column&>(column.getColumnName(), column));
                  auto name = target->alias.empty() ? column.getColumnName() : target->alias;
                  context->currentScope->targetInfo.map(name, std::make_shared<ast::ColumnInfo>(columnRef->scope, column));
                  break;
               }
               case ast::ExpressionClass::STAR: {
                  auto star = std::static_pointer_cast<ast::StarExpression>(target);
                  std::vector<catalog::Catalog> catalogs;
                  std::string scope;
                  std::vector<catalog::Column> columns;
                  //TODO implement x.*

                  break;
               }
               case ast::ExpressionClass::BOUND_FUNCTION: {
                  auto function = std::static_pointer_cast<ast::BoundFunctionExpression>(parsedExpression);
                  auto fName = function->alias.empty() ? function->functionName : function->alias;
                  if (!context->currentScope->functionsEntry.contains(fName)) {
                     error("Function entry not found", function->loc)
                  }
                  break;
               }
               default: error("Not implemented", target->loc);
            }
         }
         boundAstNode = drv.nf.node<ast::BoundTargetsExpression>(targetSelection->loc, boundTargetExpressions, targetColumns);
         break;
      }
      case ast::PipeOperatorType::WHERE: {
         auto whereClause = std::static_pointer_cast<ast::ParsedExpression>(pipeOperator->node);
         boundAstNode = analyzeExpression(whereClause, context);
         if (std::static_pointer_cast<ast::BoundExpression>(boundAstNode)->resultType->type.getTypeId() != catalog::LogicalTypeId::BOOLEAN) {
            error("Where clause is not a boolean expression", whereClause->loc);
         }
         break;
      }
      case ast::PipeOperatorType::AGGREGATE: {
         auto aggregationNode = std::static_pointer_cast<ast::AggregationNode>(pipeOperator->node);
         assert(aggregationNode->groupByNode);

         //TODO parse aggregations sets

         auto transFormedGroupExpressions = aggregationNode->groupByNode->group_expressions | std::views::transform([&](const auto& expr) {
                                               return analyzeExpression(expr, context);
                                            }) |
            std::ranges::to<std::vector<std::shared_ptr<ast::BoundExpression>>>();

         auto transFormedAggregationExpressions = aggregationNode->aggregations | std::views::transform([&](const auto& expr) {
                                                     auto boundExpr = analyzeExpression(expr, context);
                                                     assert(boundExpr->exprClass == ast::ExpressionClass::BOUND_FUNCTION);
                                                     return std::static_pointer_cast<ast::BoundFunctionExpression>(boundExpr);
                                                  }) |
            std::ranges::to<std::vector<std::shared_ptr<ast::BoundFunctionExpression>>>();

         auto boundGroupByNode = drv.nf.node<ast::BoundGroupByNode>(aggregationNode->groupByNode->loc, transFormedGroupExpressions);
         boundAstNode = drv.nf.node<ast::BoundAggregationNode>(pipeOperator->loc, boundGroupByNode, transFormedAggregationExpressions);

         break;
      }
      default: error("Not implemented", pipeOperator->loc);
   }
   pipeOperator->node = boundAstNode;
   return pipeOperator;
}

std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyzeTableRef(std::shared_ptr<ast::TableRef> tableRef, std::shared_ptr<SQLContext> context) {
   switch (tableRef->type) {
      case ast::TableReferenceType::BASE_TABLE: {
         auto baseTableRef = std::static_pointer_cast<ast::BaseTableRef>(tableRef);
         auto catalogEntry = catalog->getTypedEntry<catalog::TableCatalogEntry>(baseTableRef->tableName);
         if (!catalogEntry.has_value()) {
            error("No Catalog found with name " + baseTableRef->tableName, baseTableRef->loc);
         }
         auto boundBaseTableRef = drv.nf.node<ast::BoundBaseTableRef>(baseTableRef->loc, catalogEntry.value(), baseTableRef->alias);
         //Add to current scope
         auto tableName = baseTableRef->alias.empty() ? baseTableRef->tableName : baseTableRef->alias;
         context->currentScope->tables.emplace(std::pair{tableName, catalogEntry.value()});
         return boundBaseTableRef;
         break;
      }
      case ast::TableReferenceType::JOIN: {
         error("Not implemented", tableRef->loc);
         auto join = std::static_pointer_cast<ast::JoinRef>(tableRef);
         if (join->left) {
            analyze(join->left, context);
         } else {
            error("Left side of join is empty", join->loc);
         }
         if (join->right) {
            analyze(join->right, context);
         } else {
            error("Right side of join is empty", join->loc);
         }

         if (std::holds_alternative<std::shared_ptr<ast::ParsedExpression>>(join->condition)) {
            auto condition = std::get<std::shared_ptr<ast::ParsedExpression>>(join->condition);
            if (condition) {
               analyzeExpression(condition, context);
            }
         } else {
            error("Not implemented", join->loc);
         }
         break;
      }
      case ast::TableReferenceType::SUBQUERY: {
         error("Not implemented", tableRef->loc);
         auto subquery = std::static_pointer_cast<ast::SubqueryRef>(tableRef);
         context->pushNewScope();
         analyze(subquery->subSelectNode, context);
         break;
      }

      default: error("Not implemented", tableRef->loc);
   }
}

std::shared_ptr<ast::BoundResultModifier> SQLQueryAnalyzer::analyzeResultModifier(std::shared_ptr<ast::ResultModifier> resultModifier, std::shared_ptr<SQLContext> context) {
   switch (resultModifier->modifierType) {
      case ast::ResultModifierType::ORDER_BY: {
         auto orderByModifier = std::static_pointer_cast<ast::OrderByModifier>(resultModifier);
         std::vector<std::shared_ptr<ast::BoundOrderByElement>> boundOrderByElements{};
         for (auto orderByElement : orderByModifier->orderByElements) {
            if (orderByElement->expression) {
               auto boundExpression = analyzeExpression(orderByElement->expression, context);
               auto boundOrderByElement = drv.nf.node<ast::BoundOrderByElement>(orderByElement->loc, orderByElement->type, orderByElement->nullOrder, boundExpression);
               boundOrderByElements.push_back(boundOrderByElement);
            }
         }
         return drv.nf.node<ast::BoundOrderByModifier>(resultModifier->loc, boundOrderByElements, resultModifier->input);
      }
      default: error("Not implemented", resultModifier->loc);
   }
}

/*
 * Expressions
*/

std::shared_ptr<ast::BoundExpression> SQLQueryAnalyzer::analyzeExpression(std::shared_ptr<ast::ParsedExpression> rootNode, std::shared_ptr<SQLContext> context) {
   switch (rootNode->exprClass) {
      case ast::ExpressionClass::CONSTANT: {
         auto constExpr = std::static_pointer_cast<ast::ConstantExpression>(rootNode);
         if (!constExpr->value) {
            throw std::runtime_error("Value of constExpr is empty");
         }
         catalog::Type type = catalog::Type::int64();
         switch (constExpr->value->type) {
            case ast::ConstantType::INT:
               type = catalog::Type::int64();
               break;
            case ast::ConstantType::STRING:
               type = catalog::Type::stringType();
               break;
            case ast::ConstantType::INTERVAL:
               //TODO hardcoded
               type = catalog::Type::intervalDaytime();
               break;
            default:
               error("Not implemented", constExpr->loc);
         }
         return drv.nf.node<ast::BoundConstantExpression>(constExpr->loc, type, constExpr->value);
      }
      case ast::ExpressionClass::COLUMN_REF: {
         auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(rootNode);
         return analyzeColumnRefExpression(columnRef, context);
      }
      case ast::ExpressionClass::STAR: {
         auto star = std::static_pointer_cast<ast::StarExpression>(rootNode);
         std::vector<catalog::Catalog> catalogs;
         std::string scope;
         std::vector<std::pair<std::string, catalog::Column>> columns{};
         if (star->relationName.empty()) {
            columns = std::move(context->getColumns());
         } else {
            columns = std::move(context->getColumns(star->relationName));
         }
         auto boundStar = drv.nf.node<ast::BoundStarExpression>(star->loc, scope, columns);
         return boundStar;
      }
      case ast::ExpressionClass::COMPARISON: {
         auto comparison = std::static_pointer_cast<ast::ComparisonExpression>(rootNode);
         auto left = analyzeExpression(comparison->left, context);
         auto right = analyzeExpression(comparison->right, context);
         if (!left->resultType.has_value()) {
            error("Left side of comparison is not a valid expression", comparison->left->loc);
         }
         if (!right->resultType.has_value()) {
            error("Right side of comparison is not a valid expression", comparison->right->loc);
         }
         getCommonType(left->resultType.value().type, right->resultType.value().type);


         auto boundComparison = drv.nf.node<ast::BoundComparisonExpression>(comparison->loc, comparison->type, left, right);
         return boundComparison;
      }
      case ast::ExpressionClass::CONJUNCTION: {
         error("Not implemented", rootNode->loc);
         auto conjunction = std::static_pointer_cast<ast::ConjunctionExpression>(rootNode);
         for (auto expr : conjunction->children) {
            analyzeExpression(expr, context);
            if (!expr->resultType.has_value() || expr->resultType.value().getTypeId() != catalog::Type::boolean().getTypeId()) {
               error("Conjunction is not possible with children of type boolean", expr->loc);
            }
         }
         conjunction->resultType = catalog::Type::boolean();
         break;
      }
      case ast::ExpressionClass::OPERATOR: {
         auto operatorExpr = std::static_pointer_cast<ast::OperatorExpression>(rootNode);

         if (operatorExpr->children.size() == 0) {
            error("Operator expression has no children", operatorExpr->loc);
         }
         auto boundChildren = operatorExpr->children | std::ranges::views::transform([&](auto c) {
                                 return analyzeExpression(c, context);
                              });
         //TODO determine common type instead of take one. For exampel int32-int64=>int64
         auto resultType = std::find_if(boundChildren.begin(), boundChildren.end(), [](auto c) {
            return !c->resultType.has_value();
         });
         if (resultType != boundChildren.end()) {
            error("Operator expression has children with different types", boundChildren[0]->loc);
         }
         //Get common type
         //TODO BETTER, maybe create directly mlir::TYPE
         auto types = boundChildren | std::ranges::views::transform([](std::shared_ptr<ast::BoundExpression> c) {
                         return c->resultType.value().type;
                      }) |
            std::ranges::to<std::vector<catalog::Type>>();
         auto commonType = getCommonBaseType(types);

         return drv.nf.node<ast::BoundOperatorExpression>(operatorExpr->loc, operatorExpr->type, commonType, boundChildren | std::ranges::to<std::vector<std::shared_ptr<ast::BoundExpression>>>());
      }
      case ast::ExpressionClass::FUNCTION: {
         auto function = std::static_pointer_cast<ast::FunctionExpression>(rootNode);
         std::vector<std::shared_ptr<ast::BoundExpression>> boundArguments{};
         if (rootNode->type == ast::ExpressionType::AGGREGATE) {
            //TODO Better

            if (function->arguments.size() > 1) {
               error("Aggregation with more than one argument not supported", function->loc);
            }
            for (auto arg : function->arguments) {
               boundArguments.push_back(analyzeExpression(arg, context));
            }

            //TODO Check for correct value
            if (function->functionName == "sum" || function->functionName == "avg") {
               if (function->arguments.size() > 1) {
                  error("Aggregation with more than one argument not supported", function->loc);
               }
               if (!boundArguments[0]->resultType.has_value()) {
                  error("Argument of aggregation function is not a valid expression", boundArguments[0]->loc);
               }
               if (boundArguments[0]->resultType.value().type.getTypeId() != catalog::LogicalTypeId::INT &&
                   boundArguments[0]->resultType.value().type.getTypeId() != catalog::LogicalTypeId::FLOAT &&
                   boundArguments[0]->resultType.value().type.getTypeId() != catalog::LogicalTypeId::DECIMAL &&
                   boundArguments[0]->resultType.value().type.getTypeId() != catalog::LogicalTypeId::DOUBLE) {
                  error("AVG function needs argument of type int or float", function->loc);
               }
            } else {
               error("Not implemented", rootNode->loc);
            }

            auto scope = createTmpScope();
            auto fName = function->alias.empty() ? function->functionName : function->alias;
            context->currentScope->functionsEntry.emplace(fName, std::make_shared<ast::FunctionInfo>(scope, fName));
            return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, boundArguments[0]->resultType.value().type, function->functionName, scope, function->alias, boundArguments);
         } else {
            //TODO hardcoded
            if (function->functionName == "date") {
               if (function->arguments.size() != 1) {
                  error("Function date needs exactly one argument", function->loc);
               }
               auto arg = analyzeExpression(function->arguments[0], context);
               if (arg->resultType.has_value() && arg->resultType.value().type.getTypeId() != catalog::Type::stringType().getTypeId()) {
                  error("Function date needs argument of type string", function->loc);
               }
               auto scope = createTmpScope();
               auto fName = function->alias.empty() ? function->functionName : function->alias;
               context->currentScope->functionsEntry.emplace(fName, std::make_shared<ast::FunctionInfo>(scope, fName));
               return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, catalog::Type(catalog::LogicalTypeId::DATE, std::make_shared<catalog::DateTypeInfo>(catalog::DateTypeInfo::DateUnit::DAY)), function->functionName, "", function->alias, std::vector{arg});
            }
            if (function->functionName == "count") {
               if (function->arguments.size() != 1 && !function->star) {
                  error("Function count needs exactly one argument", function->loc);
               }
               if (!function->star && (function->arguments[0]->type != ast::ExpressionType::COLUMN_REF && function->arguments[0]->type != ast::ExpressionType::STAR)) {
                  error("Function count needs argument of type column or star", function->loc);
               }
               auto scope = createTmpScope();
               auto fName = function->alias.empty() ? function->functionName : function->alias;
               context->currentScope->functionsEntry.emplace(fName, std::make_shared<ast::FunctionInfo>(scope, fName));
               if (function->star) {
                  return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, catalog::Type::int64(), function->functionName, "", function->alias, std::vector<std::shared_ptr<ast::BoundExpression>>{});
               }
               auto arg = analyzeExpression(function->arguments[0], context);

               return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, catalog::Type::int64(), function->functionName, "", function->alias, std::vector{arg});
            }
            throw std::runtime_error("FunctionType Not implemented");
         }
         break;
      }
      default: error("Not implemented", rootNode->loc);
   }
}

std::shared_ptr<ast::BoundColumnRefExpression> SQLQueryAnalyzer::analyzeColumnRefExpression(std::shared_ptr<ast::ColumnRefExpression> columnRef, std::shared_ptr<SQLContext> context) {
   //new implementation which uses the new concept of TableProducers
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
      if (columnRef->column_names.size() == 1) {
         error("No column found with name TODO check function " + columnName, columnRef->loc);
      } else {
         error("No column found with name TODO check function " + columnRef->column_names[0] + "." + columnName, columnRef->loc);
      }
   }
   if (columns.size() > 1) {
      error(columnName + " is ambiguous", columnRef->loc);
   }



   return drv.nf.node<ast::BoundColumnRefExpression>(columnRef->loc, scope, catalog::NullableType(columns.at(0).getLogicalType(), columns.at(0).getIsNullable()), columns.at(0));
}

catalog::Type SQLQueryAnalyzer::getCommonType(catalog::Type type1, catalog::Type type2) {
   if (type1.getTypeId() == type2.getTypeId()) {
      return type1;
   }
   //TODO implement
   if (type1.getTypeId() == catalog::LogicalTypeId::DATE && type2.getTypeId() == catalog::LogicalTypeId::INTERVAL || type1.getTypeId() == catalog::LogicalTypeId::INTERVAL && type2.getTypeId() == catalog::LogicalTypeId::DATE) {
      return catalog::Type(catalog::LogicalTypeId::DATE, std::make_shared<catalog::DateTypeInfo>(catalog::DateTypeInfo::DateUnit::DAY));
   }

   if (type1.getTypeId() == catalog::LogicalTypeId::INT && type2.getTypeId() == catalog::LogicalTypeId::DECIMAL) {
      return type2;
   }
   if (type1.getTypeId() == catalog::LogicalTypeId::DECIMAL && type2.getTypeId() == catalog::LogicalTypeId::INT) {
      return type1;
   }

   throw std::runtime_error("No common type found for " + type1.toString() + " and " + type2.toString());
}
catalog::Type SQLQueryAnalyzer::getCommonBaseType(std::vector<catalog::Type> types) {
   auto commonType = types.back();
   types.pop_back();
   for (auto type : types) {
      commonType = getCommonType(commonType, type);
   }
   return commonType;
}

} // namespace lingodb::analyzer