#include "lingodb/compiler/frontend/sql_analyzer.h"


#include "lingodb/compiler/frontend/ast/bound/bound_aggregation.h"
#include "lingodb/compiler/frontend/ast/bound/bound_extend_node.h"
#include "lingodb/compiler/frontend/ast/bound/bound_groupby.h"
#include "lingodb/compiler/frontend/ast/bound/bound_tableref.h"
#include "lingodb/runtime/RecordBatchInfo.h"

#include <ranges>
namespace lingodb::analyzer {
using ResolverScope = llvm::ScopedHashTable<std::string, std::shared_ptr<ast::NamedResult>, StringInfo>::ScopeTy;
SQLQueryAnalyzer::SQLQueryAnalyzer(std::shared_ptr<catalog::Catalog> catalog) : catalog(std::move(catalog)) {
}
std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyzeAndTransform(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context) {
   auto transformed = transform(rootNode, std::make_shared<ASTTransformContext>());
   ast::NodeIdGenerator idGen{};
   std::cout << std::endl
             << std::endl;
   std::cout << "digraph ast {" << std::endl;
   std::cout << transformed->toDotGraph(1, idGen) << std::endl;
   std::cout << "}" << std::endl;
   context->pushNewScope();
   auto scope = context->createResolverScope();
  transformed = analyze(transformed, context, scope);
   return transformed;
}
std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyze(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context, ResolverScope& resolverScope) {

   switch (rootNode->nodeType) {
      case ast::NodeType::PIPE_OP: {
         auto pipeOp = std::static_pointer_cast<ast::PipeOperator>(rootNode);
         if (pipeOp->input) {
            pipeOp->input = analyze(pipeOp->input, context, resolverScope);
         }
         return analyzePipeOperator(pipeOp, context, resolverScope);
      }
      case ast::NodeType::TABLE_REF: {
         auto tableRef = std::static_pointer_cast<ast::TableRef>(rootNode);
         return analyzeTableRef(tableRef, context, resolverScope);
      }
      case ast::NodeType::RESULT_MODIFIER: {
         auto resultModifier = std::static_pointer_cast<ast::ResultModifier>(rootNode);
         if (resultModifier->input) {
            resultModifier->input = analyze(resultModifier->input, context, resolverScope);
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

               auto extendPipeOp = drv.nf.node<ast::PipeOperator>(selectNode->select_list->loc, ast::PipeOperatorType::EXTEND, context->extendNode);
               extendPipeOp->input = transformed;
               transformed = extendPipeOp;
               //Transform where_clause
               if (selectNode->where_clause) {
                  auto pipe = drv.nf.node<ast::PipeOperator>(selectNode->where_clause->loc, ast::PipeOperatorType::WHERE, selectNode->where_clause);
                  auto transFormededWhereClause = transformCast<ast::PipeOperator>(pipe, context);
                  transFormededWhereClause->input = transformed;
                  selectNode->where_clause = nullptr;
                  transformed = transFormededWhereClause;
               }

               auto aggPipeNode = drv.nf.node<ast::PipeOperator>(selectNode->loc, ast::PipeOperatorType::AGGREGATE, context->aggregationNode);
               auto transFormedAggregation = transformCast<ast::PipeOperator>(aggPipeNode, context);
               transFormedAggregation->input = transformed;
               transformed = transFormedAggregation;

               //Transform target selection
               auto select_list = selectNode->select_list;
               if (select_list) {
                  auto pipe = drv.nf.node<ast::PipeOperator>(select_list->loc, ast::PipeOperatorType::SELECT, select_list);
                  auto transformedSelect = transformCast<ast::PipeOperator>(pipe, context);
                  transformedSelect->input = transformed;
                  transformed = transformedSelect;
                  selectNode->select_list = nullptr;
               }

               //Transform Group by
               if (selectNode->groups) {
                  auto loc = selectNode->groups->loc;
                  context->aggregationNode->groupByNode = std::move(selectNode->groups);
               }

               //Transform modifiers
               for (auto modifier : selectNode->modifiers) {
                  auto transformedModifier = transformCast<ast::ResultModifier>(modifier, context);
                  transformedModifier->input = transformed;
                  transformed = transformedModifier;
               }
               selectNode->modifiers.clear();

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
               std::vector<std::shared_ptr<ast::ParsedExpression>> toRemove{};
               int i = 0;
               std::ranges::transform(selectNode->targets, selectNode->targets.begin(), [&](auto& target) {
                  return transformParsedExpression(target, context);
               });
               for (auto& target : selectNode->targets) {

                  if (target->exprClass == ast::ExpressionClass::FUNCTION) {
                     auto function = std::static_pointer_cast<ast::FunctionExpression>(target);
                     if (target->type == ast::ExpressionType::AGGREGATE) {
                        context->aggregationNode->aggregations.push_back(function);
                     } else {
                        context->extendNode->extensions.push_back(function);
                     }
                     //TODO better
                     if (function->alias.empty()) {
                        //TODO make unique alias
                        function->alias = function->functionName + "_" + std::to_string(i);
                     }
                     toRemove.emplace_back(target);
                     i++;
                  }
               }

               for (auto& target : toRemove) {
                  std::replace_if(selectNode->targets.begin(), selectNode->targets.end(), [&target](const auto& t) { return t == target; }, drv.nf.node<ast::ColumnRefExpression>(target->loc, target->alias));
               }

               return pipeOp;
            }
            case ast::PipeOperatorType::WHERE: {
               assert(pipeOp->node->nodeType == ast::NodeType::EXPRESSION);
               pipeOp->node = transformParsedExpression(std::static_pointer_cast<ast::ParsedExpression>(pipeOp->node), context);
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
               auto transformedSubSelectNode = transform(subquery->subSelectNode, std::make_shared<ASTTransformContext>());

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

std::shared_ptr<ast::ParsedExpression> SQLQueryAnalyzer::transformParsedExpression(std::shared_ptr<ast::ParsedExpression> rootNode, std::shared_ptr<ASTTransformContext> context) {
   switch (rootNode->exprClass) {
      case ast::ExpressionClass::SUBQUERY: {
         auto subqueryExpr = std::static_pointer_cast<ast::SubqueryExpression>(rootNode);
         subqueryExpr->subquery = transformCast<ast::TableProducer>(subqueryExpr->subquery, std::make_shared<ASTTransformContext>());
         return subqueryExpr;
      }
      case ast::ExpressionClass::OPERATOR: {
         auto operatorExpr = std::static_pointer_cast<ast::OperatorExpression>(rootNode);
         std::ranges::transform(operatorExpr->children, operatorExpr->children.begin(), [&](auto& child) {
            return transformParsedExpression(child, context);
         });
         return operatorExpr;
      }
      case ast::ExpressionClass::CONJUNCTION: {
         auto conjunctionExpr = std::static_pointer_cast<ast::ConjunctionExpression>(rootNode);

         std::ranges::transform(conjunctionExpr->children, conjunctionExpr->children.begin(), [&](auto& child) {
            return transformParsedExpression(child, context);
         });
         return conjunctionExpr;
      }
      case ast::ExpressionClass::COMPARISON: {
         auto comparisonExpr = std::static_pointer_cast<ast::ComparisonExpression>(rootNode);
         if (comparisonExpr->left) {
            comparisonExpr->left = transformParsedExpression(comparisonExpr->left, context);
         }
         std::ranges::transform(comparisonExpr->rightChildren, comparisonExpr->rightChildren.begin(), [&](auto& child) {
            return transformParsedExpression(child, context);
         });

         return comparisonExpr;
      }
      case ast::ExpressionClass::FUNCTION: {
         auto functionExpr = std::static_pointer_cast<ast::FunctionExpression>(rootNode);

         if (functionExpr->type == ast::ExpressionType::AGGREGATE) {
            if (functionExpr->alias.empty()) {
               //TODO make unique alias
               functionExpr->alias = functionExpr->functionName + "_" + std::to_string(0);
            }
            auto columnRef = drv.nf.node<ast::ColumnRefExpression>(functionExpr->loc, functionExpr->alias);
            context->aggregationNode->aggregations.push_back(functionExpr);

            return columnRef;

         }
      }
      default: return rootNode;
   }
}

template <class T>
std::shared_ptr<T> SQLQueryAnalyzer::transformCast(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<ASTTransformContext> context) {
   return std::static_pointer_cast<T>(transform(rootNode, context));
}

std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyzePipeOperator(std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<SQLContext> context, ResolverScope& resolverScope) {
   std::shared_ptr<ast::AstNode> boundAstNode = pipeOperator->node;
   switch (pipeOperator->pipeOpType) {
      case ast::PipeOperatorType::SELECT: {
         assert(pipeOperator->node->nodeType == ast::NodeType::EXPRESSION);
         auto targetSelection = std::static_pointer_cast<ast::TargetsExpression>(pipeOperator->node);
         std::vector<std::shared_ptr<ast::BoundExpression>> boundTargetExpressions{};
         std::vector<std::shared_ptr<ast::NamedResult>> targetColumns{};
         for (auto& target : targetSelection->targets) {
            auto parsedExpression = analyzeExpression(target, context, resolverScope);
            switch (parsedExpression->exprClass) {
               case ast::ExpressionClass::BOUND_COLUMN_REF: {
                  //ADD column_ref to targetInfo for the current scope!
                  auto columnRef = std::static_pointer_cast<ast::BoundColumnRefExpression>(parsedExpression);
                  targetColumns.emplace_back(columnRef->namedResult);
                  if (!columnRef->alias.empty()) {
                     context->mapAttribute(resolverScope, columnRef->alias, columnRef->namedResult);
                  }

                  /*auto name = target->alias.empty() ? column.getColumnName() : target->alias;
                  context->currentScope->targetInfo.map(name, std::make_shared<ast::ColumnInfo>(columnRef->scope, column));*/
                  context->currentScope->targetInfo.add(columnRef->namedResult);
                  break;
               }
               case ast::ExpressionClass::BOUND_STAR: {
                  auto star = std::static_pointer_cast<ast::BoundStarExpression>(parsedExpression);
                  std::vector<catalog::Catalog> catalogs;
                  std::string scope;
                  std::vector<catalog::Column> columns;
                  //TODO implement x.*
                  for (auto [scope, namedResult] : star->namedResults) {
                     targetColumns.emplace_back(namedResult);
                     context->currentScope->targetInfo.add(namedResult);
                  }

                  break;
               }
               case ast::ExpressionClass::BOUND_FUNCTION: {
                  error("Not implemented", target->loc);

                 auto function = std::static_pointer_cast<ast::BoundFunctionExpression>(parsedExpression);
                  auto fName = function->alias.empty() ? function->functionName : function->alias;
                  assert(function->functionInfo && function->namedResult.has_value());
                  context->currentScope->targetInfo.add(function->functionInfo);
                  break;
               }
               case ast::ExpressionClass::BOUND_OPERATOR: {
                  auto operatorExpr = std::static_pointer_cast<ast::BoundOperatorExpression>(parsedExpression);

                  auto n = std::make_shared<ast::NamedResult>(ast::NamedResultType::EXPRESSION, operatorExpr->alias, operatorExpr->resultType.value(), createTmpScope());
                  n->displayName = operatorExpr->alias.empty() ? "" : operatorExpr->alias;
                  assert(operatorExpr->resultType.has_value());
                  context->mapAttribute(resolverScope, operatorExpr->alias.empty() ? n->name : operatorExpr->alias, n);
                  targetColumns.emplace_back(n);
                  context->currentScope->targetInfo.add(n);
                  context->currentScope->evalBeforeAggr.emplace_back(operatorExpr);
                  operatorExpr->namedResult = n;

                  break;
               }
               default: error("Not implemented", target->loc);
            }
         }
         boundAstNode = drv.nf.node<ast::BoundTargetsExpression>(targetSelection->loc, targetSelection->alias,  boundTargetExpressions, targetColumns);
         break;
      }
      case ast::PipeOperatorType::WHERE: {
         //TODO check if correct
         auto whereResolverScope = context->createResolverScope();
         auto whereClause = std::static_pointer_cast<ast::ParsedExpression>(pipeOperator->node);
         boundAstNode = analyzeExpression(whereClause, context, whereResolverScope);
         if (std::static_pointer_cast<ast::BoundExpression>(boundAstNode)->resultType->type.getTypeId() != catalog::LogicalTypeId::BOOLEAN) {
            error("Where clause is not a boolean expression", whereClause->loc);
         }
         break;
      }
      case ast::PipeOperatorType::AGGREGATE: {
         auto aggregationNode = std::static_pointer_cast<ast::AggregationNode>(pipeOperator->node);

         //TODO parse aggregations sets
         std::vector<std::shared_ptr<ast::BoundExpression>> transFormedGroupExpressions{};
         if (aggregationNode->groupByNode) {
            std::ranges::transform(aggregationNode->groupByNode->group_expressions, std::back_inserter(transFormedGroupExpressions), [&](auto expr) {
               return analyzeExpression(expr, context, resolverScope);
            });
         }

         std::vector<std::shared_ptr<ast::BoundFunctionExpression>> boundAggregationExpressions{};

         std::ranges::transform(aggregationNode->aggregations, std::back_inserter(boundAggregationExpressions), [&](auto expr) {
            auto boundExpr = analyzeExpression(expr, context, resolverScope);
            assert(boundExpr->exprClass == ast::ExpressionClass::BOUND_FUNCTION);
            //boundExpr->namedResult = std::make_shared<ast::FunctionInfo>("boundExpr->scope", "boundExpr->alias", boundExpr->resultType.value());
            return std::static_pointer_cast<ast::BoundFunctionExpression>(boundExpr);
         });
         auto mapName = createMapName();
         auto boundGroupByNode = drv.nf.node<ast::BoundGroupByNode>(aggregationNode->groupByNode ? aggregationNode->groupByNode->loc : aggregationNode->loc, transFormedGroupExpressions);
         std::vector<std::shared_ptr<ast::BoundExpression>> toMap{};
         for (auto& aggr : boundAggregationExpressions) {
            if (aggr->arguments.empty() || aggr->arguments[0]->type == ast::ExpressionType::BOUND_COLUMN_REF) {
               continue;
            }
            toMap.emplace_back(aggr->arguments[0]);
            aggr->arguments[0]->alias = createTmpScope();
            aggr->arguments[0]->namedResult = std::make_shared<ast::NamedResult>(ast::NamedResultType::EXPRESSION, mapName, aggr->arguments[0]->resultType.value(), aggr->arguments[0]->alias);
         }
         //ADD to TargetInfo, see Google PIPE sql paper!
         //Maybe Not the best way!
         for (auto& aggr : boundAggregationExpressions) {
            assert(aggr->resultType.has_value());
            //TODO context->currentScope->targetInfo.map(aggr->scope, std::make_shared<ast::FunctionInfo>(aggr->scope, aggr->aliasOrUniqueIdentifier, aggr->resultType.value()));
         }
         if (!aggregationNode->groupByNode || aggregationNode->groupByNode->group_expressions.empty()) {
            for (auto boundAggr : boundAggregationExpressions) {
               boundAggr->resultType->isNullable = true;
            }
         }
         boundAstNode = drv.nf.node<ast::BoundAggregationNode>(pipeOperator->loc,boundGroupByNode, boundAggregationExpressions, toMap, mapName );

         break;
      }
      case ast::PipeOperatorType::EXTEND: {
         assert(pipeOperator->node->nodeType == ast::NodeType::EXTEND_NODE);
         auto extendNode = std::static_pointer_cast<ast::ExtendNode>(pipeOperator->node);
         std::vector<std::shared_ptr<ast::BoundExpression>> boundExtensions;
         std::ranges::transform(extendNode->extensions, std::back_inserter(boundExtensions), [&](auto& expr) {
            return analyzeExpression(expr, context, resolverScope);
         });
         boundAstNode =  drv.nf.node<ast::BoundExtendNode>(extendNode->loc, createMapName(), std::move(boundExtensions));
         break;
      }
      default: error("Not implemented", pipeOperator->loc);
   }
   pipeOperator->node = boundAstNode;
   return pipeOperator;
}

std::shared_ptr<ast::TableProducer> SQLQueryAnalyzer::analyzeTableRef(std::shared_ptr<ast::TableRef> tableRef, std::shared_ptr<SQLContext> context, ResolverScope& resolverScope) {
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
         context->mapAttribute(resolverScope, tableName, catalogEntry.value());
         return boundBaseTableRef;
         break;
      }
      case ast::TableReferenceType::JOIN: {

         auto join = std::static_pointer_cast<ast::JoinRef>(tableRef);
         switch (join->type) {
            case ast::JoinType::CROSS: {
               std::shared_ptr<ast::TableProducer> left, right;
               std::vector<std::pair<std::string, std::shared_ptr<ast::NamedResult>>> mapping{};
               if (join->left) {
                  auto leftResolverScope = context->createResolverScope();
                  auto defineScope = context->createDefineScope();
                  left = analyze(join->left, context, leftResolverScope);
                  auto localMapping = context->getTopDefinedColumns();
                  for (auto& [name, column] : localMapping) {
                     mapping.push_back({name, column});
                  }
               } else {
                  error("Left side of join is empty", join->loc);
               }
               if (join->right) {

                  auto rightResolverScope = context->createResolverScope();
                  auto defineScope = context->createDefineScope();
                  right = analyze(join->right, context, rightResolverScope);
                  auto localMapping = context->getTopDefinedColumns();
                  for (auto& [name, column] : localMapping) {
                     mapping.push_back({name, column});
                  }
               } else {
                  error("Right side of join is empty", join->loc);
               }

               for (auto& [name, column] : mapping) {
                  context->mapAttribute(resolverScope, name, column);
               }

               return drv.nf.node<ast::BoundJoinRef>(join->loc, join->type, join->refType, left, right, nullptr);

            }
            case ast::JoinType::LEFT: {
               std::shared_ptr<ast::TableProducer> left, right;
               std::vector<std::pair<std::string, std::shared_ptr<ast::NamedResult>>> mapping{};

               left = analyze(join->left, context, resolverScope);
               auto rightContext = std::make_shared<SQLContext>();
               auto rightResolverScope = rightContext->createResolverScope();
               right = analyze(join->right, rightContext, rightResolverScope);
               mapping = rightContext->getTopDefinedColumns();

               for (auto x : mapping) {
                  context->mapAttribute(resolverScope, x.first, x.second);
               }
               std::shared_ptr<ast::BoundExpression> boundCondition;
               {
                  auto predScope = context->createResolverScope();
                  if (!std::holds_alternative<std::shared_ptr<ast::ParsedExpression>>(join->condition)) {
                     error("Not implemented", join->loc);
                  }
                  boundCondition = analyzeExpression(std::get<std::shared_ptr<ast::ParsedExpression>>(join->condition), context, resolverScope);
               }
               //TODO
              /* static size_t id = 0;
               std::vector<mlir::Attribute> outerJoinMapping;
               std::string outerjoinName;
               if (!mapping.empty()) {
                  outerjoinName = "oj" + std::to_string(id++);
                  std::unordered_map<const tuples::Column*, const tuples::Column*> remapped;
                  for (auto x : mapping) {
                     if (!remapped.contains(x.second)) {
                        auto [scopename, name] = attrManager.getName(x.second);

                        auto attrDef = attrManager.createDef(outerjoinName, name, builder.getArrayAttr({attrManager.createRef(x.second)}));
                        attrDef.getColumn().type = mlir::isa<db::NullableType>(x.second->type) ? x.second->type : db::NullableType::get(builder.getContext(), x.second->type);
                        outerJoinMapping.push_back(attrDef);
                        remapped.insert({x.second, &attrDef.getColumn()});
                     }
                     context.mapAttribute(scope, x.first, remapped[x.second]);
                     context.removeFromDefinedColumns(x.second);
                  }
               }*/
               std::vector<std::shared_ptr<ast::NamedResult>> outerJoinMapping;
               std::string outerjoinName;
               static size_t id = 0;
               if (!mapping.empty()) {
                  outerjoinName = "oj" + std::to_string(id++);
                  std::unordered_map<std::shared_ptr<ast::NamedResult>, std::shared_ptr<ast::NamedResult>> remapped;
                  for (auto x : mapping) {
                     if (!remapped.contains(x.second)) {
                        auto scope = x.second->scope;
                        auto name = x.second->name;
                        auto namedResult = std::make_shared<ast::NamedResult>(x.second->type, outerjoinName, x.second->resultType, name);
                        outerJoinMapping.push_back(namedResult);
                        remapped.insert({x.second, namedResult});
                     }
                  }


               }


               auto boundJoin = drv.nf.node<ast::BoundJoinRef>(join->loc, join->type, join->refType, left, right, boundCondition);
               boundJoin->outerJoinMapping = outerJoinMapping;
               return boundJoin;

            }
               default: error("Not implemented", join->loc);
         }
         break;
      }
      case ast::TableReferenceType::SUBQUERY: {
         auto subquery = std::static_pointer_cast<ast::SubqueryRef>(tableRef);
         ast::TargetInfo targetInfo;
         std::shared_ptr<ast::TableProducer> t;
         std::shared_ptr<SQLScope> subQueryScope;
         std::vector<std::shared_ptr<ast::BoundExpression>> evalBefore;
         {
            auto subQueryResolverScope = context->createResolverScope();
            auto defineScope = context->createDefineScope();
            context->pushNewScope();
            subQueryScope = context->currentScope;
            t = analyze(subquery->subSelectNode, context, subQueryResolverScope);
            targetInfo = context->currentScope->targetInfo;
            evalBefore = context->currentScope->evalBeforeAggr;
            context->popCurrentScope();


         }
         context->currentScope->evalBeforeAggr.insert(context->currentScope->evalBeforeAggr.end(), evalBefore.begin(), evalBefore.end());

         for (auto target : targetInfo.targetColumns) {
            assert(!subquery->alias.empty());


            if (!target->displayName.empty()) {
               context->mapAttribute(resolverScope, subquery->alias + "." + target->displayName, target);
               context->mapAttribute(resolverScope,  target->displayName, target);
            } else {
               context->mapAttribute(resolverScope, subquery->alias  +"." + target->name, target);
               context->mapAttribute(resolverScope, target->name, target);
            }


         }

         return drv.nf.node<ast::BoundSubqueryRef>(subquery->loc, subQueryScope, t);

         break;
      }

      default: error("Not implemented", tableRef->loc);
   }
}

std::shared_ptr<ast::BoundResultModifier> SQLQueryAnalyzer::analyzeResultModifier(std::shared_ptr<ast::ResultModifier> resultModifier, std::shared_ptr<SQLContext> context) {
   auto resolverScope = context->createResolverScope();
   switch (resultModifier->modifierType) {
      case ast::ResultModifierType::ORDER_BY: {
         auto orderByModifier = std::static_pointer_cast<ast::OrderByModifier>(resultModifier);
         std::vector<std::shared_ptr<ast::BoundOrderByElement>> boundOrderByElements{};
         for (auto orderByElement : orderByModifier->orderByElements) {
            if (orderByElement->expression) {
               auto boundExpression = analyzeExpression(orderByElement->expression, context, resolverScope);
               auto boundOrderByElement = drv.nf.node<ast::BoundOrderByElement>(orderByElement->loc, orderByElement->type, orderByElement->nullOrder, boundExpression);
               boundOrderByElements.push_back(boundOrderByElement);
            }
         }
         return drv.nf.node<ast::BoundOrderByModifier>(resultModifier->loc, boundOrderByElements, resultModifier->input);
      }
      case ast::ResultModifierType::LIMIT: {
         auto limitModifier = std::static_pointer_cast<ast::LimitModifier>(resultModifier);
         auto limitExpression = analyzeExpression(limitModifier->limitExpression, context, resolverScope);
         if (limitExpression->exprClass != ast::ExpressionClass::BOUND_CONSTANT) {
            error("Limit expression must be a constant expression", limitModifier->loc);
         }
         if (limitExpression->resultType->type.getTypeId() != catalog::LogicalTypeId::INT) {
            error("Limit expression must be of type INT", limitModifier->loc);
         }
         return drv.nf.node<ast::BoundLimitModifier>(limitModifier->loc, limitExpression, resultModifier->input);
      }
      default: error("Not implemented", resultModifier->loc);
   }
}

/*
 * Expressions
*/

std::shared_ptr<ast::BoundExpression> SQLQueryAnalyzer::analyzeExpression(std::shared_ptr<ast::ParsedExpression> rootNode, std::shared_ptr<SQLContext> context, ResolverScope& resolverScope) {
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
            case ast::ConstantType::FLOAT: {
               auto floatConstant = std::static_pointer_cast<ast::FloatValue>(constExpr->value);
               std::string value(floatConstant->fVal);
               auto decimalPos = value.find('.');
               if (decimalPos == std::string::npos) {
                  type = catalog::Type::int64();
               } else {
                  auto s = value.size() - decimalPos - 1;
                  auto p = value.size() - 1;
                  type = catalog::Type::decimal(p, s);
               }
               break;
            }
            default:
               error("Not implemented", constExpr->loc);
         }
         return drv.nf.node<ast::BoundConstantExpression>(constExpr->loc, type, constExpr->value, constExpr->alias);
      }
      case ast::ExpressionClass::COLUMN_REF: {
         auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(rootNode);
         return analyzeColumnRefExpression(columnRef, context);
      }
      case ast::ExpressionClass::STAR: {
         auto star = std::static_pointer_cast<ast::StarExpression>(rootNode);
         std::vector<catalog::Catalog> catalogs;
         std::string relationName = star->relationName;
         std::vector<std::pair<std::string, catalog::Column>> columns{};
         auto topDefinedColumnsAll = context->getTopDefinedColumns();
         std::vector<std::pair<std::string, std::shared_ptr<ast::NamedResult>>> topDefinedColumnsWithoutDuplicates;
         //TODO better solution!!!!
         for (auto [scope, namedResult] : topDefinedColumnsAll) {
            if (std::find_if(topDefinedColumnsWithoutDuplicates.begin(), topDefinedColumnsWithoutDuplicates.end(), [&](std::pair<std::string, std::shared_ptr<ast::NamedResult>> p) {
               return (p.first == scope || star->relationName.empty()) && p.second->name == namedResult->name;
            }) == topDefinedColumnsWithoutDuplicates.end()) {
               topDefinedColumnsWithoutDuplicates.emplace_back(std::pair{scope, namedResult});
            }
         }

         auto boundStar = drv.nf.node<ast::BoundStarExpression>(star->loc, relationName, topDefinedColumnsWithoutDuplicates);
         return boundStar;
      }
      case ast::ExpressionClass::COMPARISON: {
         auto comparison = std::static_pointer_cast<ast::ComparisonExpression>(rootNode);

         if (comparison->type != ast::ExpressionType::COMPARE_IN && comparison->type != ast::ExpressionType::COMPARE_NOT_IN) {
            if (comparison->rightChildren.size() != 1) {
               error("ComparisonExpression expects exactly one right child for type: " <<  std::to_string(static_cast<int>(comparison->type)), comparison->loc)
            }

         } else {

            if (std::find_if(comparison->rightChildren.begin(), comparison->rightChildren.end(), [](auto const &child) {
               return child->exprClass != ast::ExpressionClass::CONSTANT;
            }) != comparison->rightChildren.end()) {
               error("Not implemented: IN (..,..,..) with non constant values", comparison->loc);
            }
         }

         auto left = analyzeExpression(comparison->left, context, resolverScope);
         std::vector<std::shared_ptr<ast::BoundExpression>> boundRightChildren{};
         std::ranges::transform(comparison->rightChildren, std::back_inserter(boundRightChildren), [&](auto& child) {
            return analyzeExpression(child, context, resolverScope);
         });
         if (!left->resultType.has_value()) {
            error("Left side of comparison is not a valid expression", comparison->left->loc);
         }
         std::vector<catalog::NullableType> types{};
         types.push_back(left->resultType.value());
         std::ranges::transform(boundRightChildren, std::back_inserter(types), [](auto& child) {
            return child->resultType.value();
         });

         auto commonTypes = toCommonTypes(types);
         left->resultType = commonTypes[0];
         size_t x = 1;
         for (auto boundChild : boundRightChildren) {
            boundChild->resultType = commonTypes[x];
            x++;
         }


         auto boundComparison = drv.nf.node<ast::BoundComparisonExpression>(comparison->loc, comparison->type, comparison->alias, left, boundRightChildren);
         return boundComparison;
      }
      case ast::ExpressionClass::CONJUNCTION: {
         auto conjunction = std::static_pointer_cast<ast::ConjunctionExpression>(rootNode);
         std::vector<std::shared_ptr<ast::BoundExpression>> boundChildren{};
         for (auto expr : conjunction->children) {
            auto boundExpr = analyzeExpression(expr, context, resolverScope);
            boundChildren.push_back(boundExpr);
            if (!boundExpr->resultType.has_value() || boundExpr->resultType.value().type.getTypeId() != catalog::Type::boolean().getTypeId()) {
               error("Conjunction is not possible with children of type boolean", expr->loc);
            }
         }
         return drv.nf.node<ast::BoundConjunctionExpression>(conjunction->loc, conjunction->type, conjunction->alias, boundChildren);
         break;
      }
      case ast::ExpressionClass::OPERATOR: {
         auto operatorExpr = std::static_pointer_cast<ast::OperatorExpression>(rootNode);

         if (operatorExpr->children.size() == 0) {
            error("Operator expression has no children", operatorExpr->loc);
         }
         std::vector<std::shared_ptr<ast::BoundExpression>> boundChildren{};
         std::ranges::transform(operatorExpr->children, std::back_inserter(boundChildren), [&](auto c) {
            return analyzeExpression(c, context, resolverScope);
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

         std::vector<catalog::NullableType> types{};
         std::ranges::transform(boundChildren, std::back_inserter(types), [](auto c) {
            return c->resultType.value();
         });
         auto commonType = getCommonBaseType(types, operatorExpr->type);
         auto commonNumbers = toCommonNumber(types);
         size_t t = 0;
         for (auto boundChild : boundChildren) {
            boundChild->resultType = commonNumbers[t];
            t++;
         }
         //TODO base
         return drv.nf.node<ast::BoundOperatorExpression>(operatorExpr->loc, operatorExpr->type, commonType.type, operatorExpr->alias, boundChildren);
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
               auto boundArg = analyzeExpression(arg, context, resolverScope);
               boundArguments.push_back(boundArg);

            }
            //TODO Check for correct value
            if (function->functionName == "sum" || function->functionName == "avg" || function->functionName == "min" || function->functionName == "max") {
               if (function->arguments.size() > 1) {
                  error("Aggregation with more than one argument not supported", function->loc);
               }
               if (!boundArguments[0]->resultType.has_value() && !function->star) {
                  error("Argument of aggregation function is not a valid expression", boundArguments[0]->loc);
               }
               if (boundArguments[0]->resultType.value().type.getTypeId() != catalog::LogicalTypeId::INT &&
                   boundArguments[0]->resultType.value().type.getTypeId() != catalog::LogicalTypeId::FLOAT &&
                   boundArguments[0]->resultType.value().type.getTypeId() != catalog::LogicalTypeId::DECIMAL &&
                   boundArguments[0]->resultType.value().type.getTypeId() != catalog::LogicalTypeId::DOUBLE) {
                  error("AVG function needs argument of type int or float", function->loc);
               }
               auto scope = createTmpScope();
               auto fName = function->alias.empty() ? function->functionName : function->alias;
               //Find correct resultType
               auto resultType = boundArguments[0]->resultType.value();
               if (function->functionName == "avg") {
                  //TODO type
                  if (resultType.type.getTypeId() == catalog::LogicalTypeId::INT) {
                     resultType = getCommonTypeAfterOperation(catalog::Type::decimal(19,0), catalog::Type::decimal(19,0), ast::ExpressionType::OPERATOR_DIVIDE);
                  } else if (resultType.type.getTypeId() == catalog::LogicalTypeId::DECIMAL) {
                     resultType = getCommonTypeAfterOperation(resultType, catalog::Type::decimal(19,0), ast::ExpressionType::OPERATOR_DIVIDE);
                  }
                  resultType.isNullable = boundArguments[0]->resultType->isNullable;
                  //else keep type

               }
               resultType.isNullable = true;

               auto p = resultType.type.getInfo<catalog::DecimalTypeInfo>()->getPrecision();
               //TODO here the information gets lost which mlir type the argument and therefore the function has
               auto fInfo = std::make_shared<ast::FunctionInfo>(scope, fName, resultType);
               fInfo->displayName = function->alias;
               context->mapAttribute(resolverScope, fName, fInfo);
               return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, resultType, function->functionName, scope, fName, function->distinct, boundArguments, fInfo);
            }
            //TODO better and cleaner!
            if (function->functionName == "count") {
               //TODO parse agrguments if not star!!

               if (function->arguments.size() > 1) {
                  error("Aggregation with more than one argument not supported", function->loc);
               }
               if (function->arguments.size() == 0 && !function->star) {
                  error("Argument of aggregation function is not a valid expression", boundArguments[0]->loc);
               }
               if (function->star) {
                  function->functionName = function->functionName+"*";
               }

               auto scope = createTmpScope();
               auto fName = function->alias.empty() ? function->functionName : function->alias;
               auto resultType = catalog::Type::int64();
               auto fInfo = std::make_shared<ast::FunctionInfo>(scope, fName, resultType);
               //TODO better
               fInfo->displayName = function->alias;
               context->mapAttribute(resolverScope, fName, fInfo);
               return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, resultType, function->functionName, scope, fName, function->distinct, boundArguments, fInfo);
            } else {
               error("Not implemented", rootNode->loc);
            }

         } else {
            //TODO hardcoded
            if (function->functionName == "date") {
               if (function->arguments.size() != 1) {
                  error("Function date needs exactly one argument", function->loc);
               }
               auto arg = analyzeExpression(function->arguments[0], context, resolverScope);
               if (arg->resultType.has_value() && arg->resultType.value().type.getTypeId() != catalog::Type::stringType().getTypeId()) {
                  error("Function date needs argument of type string", function->loc);
               }
               auto scope = createTmpScope();
               auto fName = function->alias.empty() ? function->functionName : function->alias;
               auto resultType = catalog::Type(catalog::LogicalTypeId::DATE, std::make_shared<catalog::DateTypeInfo>(catalog::DateTypeInfo::DateUnit::DAY));
               //TODO  context->currentScope->functionsEntry.emplace(fName, std::make_shared<ast::FunctionInfo>(scope, fName, resultType));
               return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, resultType, function->functionName, "", function->alias, function->distinct, std::vector{arg}, nullptr);
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
               auto fInfo =  std::make_shared<ast::FunctionInfo>(scope, fName, boundArguments[0]->resultType.value());

               fInfo->displayName = function->alias;
               context->mapAttribute(resolverScope, fName, fInfo);
               auto resultType = catalog::Type::int64();
               if (function->star) {
                  return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, resultType , function->functionName + "*", "", function->alias, function->distinct, std::vector<std::shared_ptr<ast::BoundExpression>>{}, fInfo);
               }
               auto arg = analyzeExpression(function->arguments[0], context, resolverScope);

               return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, resultType, function->functionName, "", function->alias, function->distinct, std::vector{arg}, fInfo);
            }
            if (function->functionName == "EXTRACT") {
               if (function->arguments.size() != 2) {
                  error("Function extract needs exactly two arguments", function->loc);
               }
               auto arg1 = analyzeExpression(function->arguments[0], context, resolverScope);
               auto arg2 = analyzeExpression(function->arguments[1], context, resolverScope);
               if (arg2->resultType.has_value() && arg2->resultType.value().type.getTypeId() != catalog::LogicalTypeId::DATE && arg2->resultType.value().type.getTypeId() != catalog::LogicalTypeId::INTERVAL) {
                  error("Function extract needs second argument of type date or interval", function->loc);
               }
               auto scope = createTmpScope();
               auto fName = function->alias.empty() ? function->functionName : function->alias;
               auto resultType = catalog::Type::int64();

               auto fInfo =  std::make_shared<ast::FunctionInfo>(scope, fName, resultType);
               fInfo->displayName = function->alias;
               context->mapAttribute(resolverScope, fName, fInfo);

               return drv.nf.node<ast::BoundFunctionExpression>(function->loc, function->type, resultType, function->functionName, scope, fName, function->distinct, std::vector{arg1, arg2}, fInfo);

            }
            throw std::runtime_error("FunctionType Not implemented");
         }
         break;
      }
      case ast::ExpressionClass::CAST: {
         auto castExpr = std::static_pointer_cast<ast::CastExpression>(rootNode);
         auto boundChild = analyzeExpression(castExpr->child, context, resolverScope);
         switch (castExpr->logicalType) {
            case ast::DATE: {
               switch (boundChild->type) {
                  case ast::ExpressionType::VALUE_CONSTANT: {
                     auto constExpr = std::static_pointer_cast<ast::BoundConstantExpression>(boundChild);
                     if (constExpr->value->type != ast::ConstantType::STRING) {
                        error("Cannot cast " + constExpr->value->toString() + " to date", constExpr->loc);
                     }
                     return drv.nf.node<ast::BoundCastExpression>(castExpr->loc, catalog::Type(catalog::LogicalTypeId::DATE, std::make_shared<catalog::DateTypeInfo>(catalog::DateTypeInfo::DateUnit::DAY)), castExpr->alias, boundChild, castExpr->logicalType, castExpr->typeMods);
                  }
                  default: error("Not implemented", rootNode->loc);
               }
            }
            case ast::LogicalType::INTERVAL: {
               auto constExpr = std::static_pointer_cast<ast::BoundConstantExpression>(boundChild);
               if (constExpr->value->type != ast::ConstantType::STRING) {
                  error("Cannot cast " + constExpr->value->toString() + " to date", constExpr->loc);
               }
               //TODO hardcoded
               //!Shortcutted here, implement different interval types later
               return drv.nf.node<ast::BoundCastExpression>(castExpr->loc, catalog::Type::intervalDaytime(), castExpr->alias, boundChild, castExpr->logicalType, castExpr->typeMods);
            }
            default: error("Not implemented", rootNode->loc);
         }
      }
      case ast::ExpressionClass::BETWEEN: {
         auto between = std::static_pointer_cast<ast::BetweenExpression>(rootNode);
         auto boundInput = analyzeExpression(between->input, context, resolverScope);
         auto boundLower = analyzeExpression(between->lower, context, resolverScope);
         auto boundUpper = analyzeExpression(between->upper, context, resolverScope);
         if (!boundInput->resultType.has_value() || !boundLower->resultType.has_value() || !boundUpper->resultType.has_value()) {
            error("Between expression has no valid type", rootNode->loc);
         }
         //Check for correct Types
         auto commonTypes = toCommonTypes({boundInput->resultType.value(), boundLower->resultType.value(), boundUpper->resultType.value()});
         boundInput->resultType = commonTypes[0];
         boundLower->resultType = commonTypes[1];
         boundUpper->resultType = commonTypes[2];
         return drv.nf.node<ast::BoundBetweenExpression>(rootNode->loc, between->type, catalog::Type::boolean(), rootNode->alias, boundInput, boundLower, boundUpper);
      }
      case ast::ExpressionClass::SUBQUERY: {
         auto subqueryExpr = std::static_pointer_cast<ast::SubqueryExpression>(rootNode);
         if (subqueryExpr->subQueryType == ast::SubqueryType::INVALID) {
            error("Should not happen, subquery type is invalid", subqueryExpr->loc);
         }
         std::shared_ptr<ast::TableProducer> boundSubquery;
         std::shared_ptr<SQLScope> subqueryScope;
         ast::TargetInfo subqueryTargetInfo;
         {
            auto subqueryResolver = context->createResolverScope();
            auto subqueryDefineScope = context->createDefineScope();
            context->pushNewScope();
            subqueryScope = context->currentScope;
            boundSubquery = analyze(subqueryExpr->subquery, context, subqueryResolver);
            subqueryTargetInfo = context->currentScope->targetInfo;
            context->popCurrentScope();
         }
         //TODO check if subquery is a valid expression and has valid type
         if (subqueryTargetInfo.targetColumns.size() != 1) {
            error("subquery expressions must produce a single value", subqueryExpr->loc);
         }
         auto namedResult = subqueryTargetInfo.targetColumns[0];
         auto resultType = namedResult->resultType;

         if (subqueryExpr->subQueryType != ast::SubqueryType::SCALAR) {
            resultType = catalog::Type::boolean();
         }
         std::shared_ptr<ast::BoundExpression> boundToTestExpr = nullptr;
         if (subqueryExpr->testExpr) {
            boundToTestExpr = analyzeExpression(subqueryExpr->testExpr, context, resolverScope);
         }

         return drv.nf.node<ast::BoundSubqueryExpression>(subqueryExpr->loc, subqueryExpr->subQueryType, resultType, subqueryExpr->alias, namedResult, subqueryScope, boundSubquery, boundToTestExpr);

      }
      default: error("Not implemented", rootNode->loc);
   }
}

std::shared_ptr<ast::BoundColumnRefExpression> SQLQueryAnalyzer::analyzeColumnRefExpression(std::shared_ptr<ast::ColumnRefExpression> columnRef, std::shared_ptr<SQLContext> context) {
   //new implementation which uses the new concept of TableProducers
   auto columnName = columnRef->column_names.size() == 1 ? columnRef->column_names[0] : columnRef->column_names[1];

   std::string scope;
   std::shared_ptr<ast::NamedResult> found;
   if (columnRef->column_names.size() == 2) {
      found = context->getNamedResultInfo(columnRef->column_names[0]+ "." + columnRef->column_names[1]);

   } else if (columnRef->column_names.size() == 1) {

      found = context->getNamedResultInfo(columnRef->column_names[0]);
   } else {
      throw std::runtime_error("Not implemented");
   }

   if (!found) {
      error("Column not found", columnRef->loc);
   }
   found->displayName = columnRef->alias.empty() ? found->displayName : columnRef->alias;
   return drv.nf.node<ast::BoundColumnRefExpression>(columnRef->loc,found->scope, found->resultType, found, columnRef->alias);
}

catalog::NullableType SQLQueryAnalyzer::getCommonType(catalog::NullableType nullableType1, catalog::NullableType nullableType2) {
   auto type1 = nullableType1.type;
   auto type2 = nullableType2.type;
   catalog::Type commonType = type1;
   //TODO implement
   if (type1.getTypeId() == type2.getTypeId()) {

   } else if (type1.getTypeId() == catalog::LogicalTypeId::DATE && type2.getTypeId() == catalog::LogicalTypeId::INTERVAL || type1.getTypeId() == catalog::LogicalTypeId::INTERVAL && type2.getTypeId() == catalog::LogicalTypeId::DATE) {
      return catalog::Type(catalog::LogicalTypeId::DATE, std::make_shared<catalog::DateTypeInfo>(catalog::DateTypeInfo::DateUnit::DAY));
   } else if (type1.getTypeId() == catalog::LogicalTypeId::INT && type2.getTypeId() == catalog::LogicalTypeId::DECIMAL) {
      commonType = type2;
   } else if (type1.getTypeId() == catalog::LogicalTypeId::DECIMAL && type2.getTypeId() == catalog::LogicalTypeId::INT) {
      commonType = type1;
   } else if (type1.getTypeId() == catalog::LogicalTypeId::CHAR && type2.getTypeId() == catalog::LogicalTypeId::STRING) {
      commonType = type2;
   } else if (type2.getTypeId() == catalog::LogicalTypeId::CHAR && type1.getTypeId() == catalog::LogicalTypeId::STRING) {
      commonType = type1;
   } else {
      throw std::runtime_error("No common type found for " + type1.toString() + " and " + type2.toString());
   }
   //TODO is this correct
   return catalog::NullableType(commonType, nullableType1.isNullable || nullableType2.isNullable);


}

std::pair<unsigned long, unsigned long> SQLQueryAnalyzer::getAdaptedDecimalPAndSAfterMulDiv(unsigned long p, unsigned long s) {
   auto beforeComma = p - s;

   if (beforeComma > 32 && s > 6) {
      p = 38;
      s = 6;
   } else if (beforeComma > 32 && s <= 6) {
      p = 28;
   } else {
      p = std::min<unsigned long>(p, 38);
      s = std::min<unsigned long>(s, 38 - beforeComma);
   }
   return  {p,s};
}
catalog::NullableType SQLQueryAnalyzer::getCommonTypeAfterOperation(catalog::NullableType type1, catalog::NullableType type2, ast::ExpressionType operationType) {
   //TODO type
   auto commonType = getCommonType(type1, type2);

   //Maybe the other way arround
   switch (operationType) {
      case ast::ExpressionType::OPERATOR_DIVIDE: {
         if (type1.type.getTypeId() == catalog::LogicalTypeId::DECIMAL && type2.type.getTypeId() == catalog::LogicalTypeId::DECIMAL) {
            auto type1Info = type1.type.getInfo<catalog::DecimalTypeInfo>();
            auto type2Info = type2.type.getInfo<catalog::DecimalTypeInfo>();



            auto [p , s] = getAdaptedDecimalPAndSAfterMulDiv(type1Info->getPrecision() - type1Info->getScale() + type2Info->getScale() + std::max<unsigned long>(6, type1Info->getScale() + type2Info->getPrecision()), std::max<unsigned long>(6, type1Info->getScale() + type2Info->getPrecision()));


            return catalog::NullableType(catalog::Type::decimal(p,s), commonType.isNullable );
         }

         return commonType;

      }
      case ast::ExpressionType::OPERATOR_TIMES: {
         if (type1.type.getTypeId() == catalog::LogicalTypeId::DECIMAL && type2.type.getTypeId() == catalog::LogicalTypeId::DECIMAL) {
            auto type1Info = type1.type.getInfo<catalog::DecimalTypeInfo>();
            auto type2Info = type2.type.getInfo<catalog::DecimalTypeInfo>();
            auto [p,s] = getAdaptedDecimalPAndSAfterMulDiv(type1Info->getPrecision() + type2Info->getPrecision(), type1Info->getScale() + type2Info->getScale());
            return catalog::NullableType(catalog::Type::decimal(p, s), commonType.isNullable);
         }


         return commonType;




      }
      default: return commonType;
   }
}

catalog::NullableType SQLQueryAnalyzer::getCommonBaseType(std::vector<catalog::NullableType> types) {
   auto commonType = types.front();
   for (size_t i = 1; i < types.size(); ++i) {
      commonType = getCommonType(commonType, types[i]);
   }
   return commonType;
}

std::vector<catalog::NullableType> SQLQueryAnalyzer::toCommonNumber(std::vector<catalog::NullableType> types) {
   auto anyDecimal = llvm::any_of(types, [](catalog::NullableType type) {return type.type.getTypeId() == catalog::LogicalTypeId::DECIMAL;});
   auto anyFloat = llvm::any_of(types, [](catalog::NullableType type) { return type.type.getTypeId() == catalog::LogicalTypeId::FLOAT; });
   if (anyDecimal && !anyFloat) {
      std::vector<catalog::NullableType> res;
      for (auto type: types) {
         if (type.type.getTypeId() != catalog::LogicalTypeId::DECIMAL) {
            type.castType = std::make_shared<catalog::NullableType>(catalog::Type::decimal(19,0));
            res.push_back(type);

         } else {
            res.push_back(type);
         }
      }
      return res;
   }

   return toCommonTypes(types);

}

std::vector<catalog::NullableType> SQLQueryAnalyzer::toCommonTypes(std::vector<catalog::NullableType> types) {
   auto commonType = getCommonBaseType(types);
   std::vector<catalog::NullableType> res;
   for (auto type : types) {
      type.castType = std::make_shared<catalog::NullableType>(commonType);
      res.push_back(type);
   }

   return res;
}

catalog::NullableType SQLQueryAnalyzer::getCommonBaseType(std::vector<catalog::NullableType> types, ast::ExpressionType operationType) {
   auto commonType = types.front();
   for (size_t i = 1; i < types.size(); ++i) {
      commonType = getCommonTypeAfterOperation(commonType, types[i], operationType);
   }
   return commonType;
}

} // namespace lingodb::analyzer