#include "lingodb/compiler/frontend/translator/sql_mlir_translator.h"

#include "lingodb/catalog/Defs.h"
#include "lingodb/catalog/MLIRTypes.h"
#include "lingodb/catalog/TableCatalogEntry.h"
#include "lingodb/compiler/Dialect/RelAlg/Transforms/queryopt/QueryGraph.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorDialect.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorOps.h"
#include "lingodb/compiler/Dialect/TupleStream/TupleStreamOps.h"
#include "lingodb/compiler/Dialect/util/UtilDialect.h"
#include "lingodb/compiler/frontend/SQL/Parser.h"
#include "lingodb/compiler/frontend/sql-parser/aggregation_node.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/comparison_expression.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/conjunction_expression.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/constant_expression.h"
#include "lingodb/compiler/frontend/sql-parser/query_node/pipe_select_node.h"
#include "lingodb/compiler/frontend/sql-parser/tableref/basetableref.h"
#include "lingodb/compiler/frontend/sql-parser/tableref/subqueryref.h"
#include "lingodb/compiler/runtime/ExecutionContext.h"
#include "lingodb/compiler/runtime/RelationHelper.h"
#include "lingodb/utility/Serialization.h"

#include <mlir-c/IR.h>
namespace lingodb::translator {
SQLMlirTranslator::SQLMlirTranslator(mlir::ModuleOp moduleOp, std::shared_ptr<catalog::Catalog> catalog) : moduleOp(moduleOp),
                                                                                                           attrManager(moduleOp->getContext()->getLoadedDialect<compiler::dialect::tuples::TupleStreamDialect>()->getColumnManager()), catalog(catalog), translationContext(std::make_shared<TranslationContext>())

{
   moduleOp.getContext()->getLoadedDialect<lingodb::compiler::dialect::util::UtilDialect>()->getFunctionHelper().setParentModule(moduleOp);
}
std::optional<mlir::Value> SQLMlirTranslator::translate(mlir::OpBuilder& builder, std::shared_ptr<ast::QueryNode> queryNode, std::shared_ptr<analyzer::SQLContext> context) {
   if (queryNode->type == ast::QueryNodeType::SELECT_NODE) {
      auto selectNode = std::static_pointer_cast<ast::SelectNode>(queryNode);
      mlir::Block* block = new mlir::Block();
      auto localTableType = translateSelectNode(builder, selectNode, context, block);
      compiler::dialect::relalg::QueryOp queryOp = builder.create<compiler::dialect::relalg::QueryOp>(builder.getUnknownLoc(), mlir::TypeRange{localTableType}, mlir::ValueRange{});
      queryOp.getQueryOps().getBlocks().clear();
      queryOp.getQueryOps().push_back(block);
      return queryOp.getResults()[0];

   } else if (queryNode->type == ast::QueryNodeType::PIPE_NODE) {
      auto pipeSelectNode = std::static_pointer_cast<ast::PipeSelectNode>(queryNode);
      mlir::Block* block = new mlir::Block();
      auto [value, localTableType] = translatePipeSelectNode(builder, pipeSelectNode, context, block);
      compiler::dialect::relalg::QueryOp queryOp = builder.create<compiler::dialect::relalg::QueryOp>(builder.getUnknownLoc(), mlir::TypeRange{localTableType}, mlir::ValueRange{});
      queryOp.getQueryOps().getBlocks().clear();
      queryOp.getQueryOps().push_back(block);
      return queryOp.getResults()[0];
   }
   return std::nullopt;
}

mlir::Type SQLMlirTranslator::translateSelectNode(mlir::OpBuilder& builder, std::shared_ptr<ast::SelectNode> selectNode, std::shared_ptr<analyzer::SQLContext> context, mlir::Block* block) {
   mlir::Type localTableType;
   {
      mlir::OpBuilder::InsertionGuard guard(builder);
      builder.setInsertionPointToStart(block);
      //FROM clause
      mlir::Value tree = translateTableRef(builder, selectNode->from_clause, context);

      // where
      tree = translateWhereClause(builder, selectNode->where_clause, context, tree);
      localTableType = tree.getType();
      //Select clause

      //Modifiers
      for (auto modifier : selectNode->modifiers) {
         tree = translateModifier(builder, modifier, context, tree);
      }

      //Group by
      if (selectNode->startPipeOperator && selectNode->startPipeOperator->type == ast::PipeOperatorType::AGGREGATE) {
         tree = translateAggregatePipeOperator(builder, selectNode->startPipeOperator, context, tree);
      }

      auto [type, tree_] = translateTargetInfo(builder, selectNode, context, tree);
      localTableType = type;
      tree = tree_;
   }
   return localTableType;
}

mlir::Value SQLMlirTranslator::translateSubSelectNode(mlir::OpBuilder& builder, std::shared_ptr<ast::SelectNode> selectNode, std::shared_ptr<analyzer::SQLContext> context) {
   //FROM clause
   mlir::Value tree = translateTableRef(builder, selectNode->from_clause, context);

   // where
   tree = translateWhereClause(builder, selectNode->where_clause, context, tree);
   //Select clause

   return tree;
}

std::pair<mlir::Value, mlir::Type> SQLMlirTranslator::translatePipeSelectNode(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeSelectNode> pipeSelectNode, std::shared_ptr<analyzer::SQLContext> context, mlir::Block* block) {
   mlir::Type localTableType;
   {
      mlir::OpBuilder::InsertionGuard guard(builder);
      builder.setInsertionPointToStart(block);
      //FROM clause
      auto start = pipeSelectNode->startPipeOperator;
      mlir::Value tree;
      if (start->node->nodeType != ast::NodeType::TABLE_REF) {
         if (start->node->nodeType == ast::NodeType::QUERY_NODE) {
            auto queryNode = std::static_pointer_cast<ast::QueryNode>(start->node);
            tree = translateSubSelectNode(builder, std::static_pointer_cast<ast::SelectNode>(queryNode), context);
         } else {
            throw std::runtime_error("should not happen");
         }
      } else {
         tree = translateTableRef(builder, std::static_pointer_cast<ast::TableRef>(start->node), context);
      }

      auto currentPipe = start->next;
      while (currentPipe) {
         tree = translatePipeOpNode(builder, pipeSelectNode, currentPipe, context, tree);
         currentPipe = currentPipe->next;
      }

      auto [type, tree_] = translateTargetInfo(builder, pipeSelectNode, context, tree);
      tree = tree_;
      localTableType = type;
      return std::make_pair(tree, localTableType);
   }
}

mlir::Value SQLMlirTranslator::translateSubPipeSelectNode(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeSelectNode> pipeSelectNode, std::shared_ptr<analyzer::SQLContext> context) {
   mlir::Type localTableType;
   {
      //FROM clause
      auto start = pipeSelectNode->startPipeOperator;
      if (start->node->nodeType != ast::NodeType::TABLE_REF) {
         throw std::runtime_error("should not happen");
      }
      mlir::Value tree = translateTableRef(builder, std::static_pointer_cast<ast::TableRef>(start->node), context);
      auto currentPipe = start->next;
      while (currentPipe) {
         tree = translatePipeOpNode(builder, pipeSelectNode, currentPipe, context, tree);
         currentPipe = currentPipe->next;
      }

      return tree;
   }
}

mlir::Value SQLMlirTranslator::translateTableRef(mlir::OpBuilder& builder, std::shared_ptr<ast::TableRef> fromClause, std::shared_ptr<analyzer::SQLContext> context) {
   mlir::Value last;
   if (fromClause->type == ast::TableReferenceType::BASE_TABLE) {
      auto baseTableRef = std::static_pointer_cast<ast::BaseTableRef>(fromClause);
      auto translated = translateBaseTableRef(builder, baseTableRef, context);
      return translated;
   }
   if (fromClause->type == ast::TableReferenceType::JOIN) {
      auto joinTableRef = std::static_pointer_cast<ast::JoinRef>(fromClause);
      auto translated = translateJoinTableRef(builder, joinTableRef, context);
      return translated;
   }
   if (fromClause->type == ast::TableReferenceType::SUBQUERY) {
      auto subQueryRef = std::static_pointer_cast<ast::SubqueryRef>(fromClause);
      assert(subQueryRef->subSelectNode->type == ast::QueryNodeType::SELECT_NODE);
      return translateSubSelectNode(builder, std::static_pointer_cast<ast::SelectNode>(subQueryRef->subSelectNode), context);
   }
   throw std::runtime_error("Not implemented");
}

/*
 * Tableref
 */
mlir::Value SQLMlirTranslator::translateBaseTableRef(mlir::OpBuilder& builder, std::shared_ptr<ast::BaseTableRef> baseTableRef, std::shared_ptr<analyzer::SQLContext> context) {
   std::string relation = baseTableRef->catalogEntry->getName();
   //std::string alias = relation;
   if (!baseTableRef->catalogEntry) {
      throw std::runtime_error("Table not found");
   }

   auto rel = baseTableRef->catalogEntry;
   std::string scopeName = baseTableRef->alias.empty() ? relation : baseTableRef->alias;
   std::vector<mlir::NamedAttribute> columns{};
   for (auto& col : rel->getColumns()) {
      auto attrDef = attrManager.createDef(scopeName, col.getColumnName());
      attrDef.getColumn().type = createTypeForColumn(builder.getContext(), col);
      columns.push_back(builder.getNamedAttr(col.getColumnName(), attrDef));
   }
   return builder.create<compiler::dialect::relalg::BaseTableOp>(builder.getUnknownLoc(), compiler::dialect::tuples::TupleStreamType::get(builder.getContext()), relation, builder.getDictionaryAttr(columns));
}
mlir::Value SQLMlirTranslator::translateJoinTableRef(mlir::OpBuilder& builder, std::shared_ptr<ast::JoinRef> join, std::shared_ptr<analyzer::SQLContext> context) {
   mlir::Value left;
   mlir::Value right;
   if (join->type == ast::JoinType::INNER) {
      if (std::holds_alternative<std::shared_ptr<ast::TableRef>>(join->left)) {
         left = translateTableRef(builder, std::get<std::shared_ptr<ast::TableRef>>(join->left), context);
      } else {
         auto pipeSelectNode = std::static_pointer_cast<ast::PipeSelectNode>(std::get<std::shared_ptr<ast::QueryNode>>(join->left));
         auto t = translateSubPipeSelectNode(builder, pipeSelectNode, context);
         left = t;
      }
      right = translateTableRef(builder, join->right, context);
      mlir::Block* pred;
      if (std::holds_alternative<std::shared_ptr<ast::ParsedExpression>>(join->condition)) {
         auto p = std::get<std::shared_ptr<ast::ParsedExpression>>(join->condition);
         pred = translatePredicate(builder, p, context);
      } else {
         throw std::runtime_error("Wrong condition type");
      }

      auto joinOp = builder.create<compiler::dialect::relalg::InnerJoinOp>(builder.getUnknownLoc(), compiler::dialect::tuples::TupleStreamType::get(builder.getContext()), left, right);
      joinOp.getPredicate().push_back(pred);
      return joinOp;
   }
   if (join->type == ast::JoinType::MARK) {
      if (join->refType == ast::JoinCondType::CROSS) {
         left = translateTableRef(builder, std::get<std::shared_ptr<ast::TableRef>>(join->left), context);
         right = translateTableRef(builder, join->right, context);
         //TODO Cross JOIN
         //auto t = builder.create<compiler::dialect::relalg::CrossProductOp>(builder.getUnknownLoc(), compiler::dialect::tuples::TupleStreamType::get(builder.getContext()), last, translated);
         return builder.create<compiler::dialect::relalg::CrossProductOp>(builder.getUnknownLoc(), compiler::dialect::tuples::TupleStreamType::get(builder.getContext()), left, right);
      } else {
         throw std::runtime_error("Not implemented join type");
      }
   }

   throw std::runtime_error("Not implemented");
}
/*
std::pair<mlir::Type, mlir::Value> SQLMlirTranslator::translateTargetSelectionList(mlir::OpBuilder& builder, std::shared_ptr<ast::TargetsExpression> targetsExpression, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   std::vector<mlir::Attribute> attrs;
   std::vector<mlir::Attribute> names;
   std::vector<mlir::Attribute> colMemberNames;
   std::vector<mlir::Attribute> colTypes;
   auto& memberManager = builder.getContext()->getLoadedDialect<compiler::dialect::subop::SubOperatorDialect>()->getMemberManager();

   for (auto& target : targetsExpression->targets) {
      if (target->type != ast::ExpressionType::COLUMN_REF) {
         throw std::runtime_error("Not implemented. Currently only direct Columnref in the select clause are supported");
      }
      auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(target);
      for (auto column : columnRef->columns) {
         auto name = target->alias.empty() ? column.getColumnName() : target->alias;
         names.push_back(builder.getStringAttr(name));
         auto colMemberName = memberManager.getUniqueMember(name.empty() ? "unnamed" : name);
         auto type = createTypeForColumn(builder.getContext(), column);
         colTypes.push_back(mlir::TypeAttr::get(type));
         colMemberNames.push_back(builder.getStringAttr(colMemberName));
         auto attrDef = attrManager.createRef(columnRef->scope, column.getColumnName());
         attrs.push_back(attrDef);
      }
   }

   mlir::Type localTableType = compiler::dialect::subop::LocalTableType::get(
       builder.getContext(),
       compiler::dialect::subop::StateMembersAttr::get(
           builder.getContext(),
           builder.getArrayAttr(colMemberNames),
           builder.getArrayAttr(colTypes)
       ),
       builder.getArrayAttr(names)
   );
   
   mlir::Value result = builder.create<compiler::dialect::relalg::MaterializeOp>(
       builder.getUnknownLoc(),
       localTableType,
       tree,
       builder.getArrayAttr(attrs),
       builder.getArrayAttr(names)
   );
   
   // Use the materialized result in the QueryReturnOp instead of the input tree
   builder.create<compiler::dialect::relalg::QueryReturnOp>(builder.getUnknownLoc(), result);
   
   return std::make_pair(localTableType, result);
}
*/
/*
std::pair<mlir::Type, mlir::Value> SQLMlirTranslator::translateTargetSelectionList(mlir::OpBuilder& builder, std::shared_ptr<ast::SelectNode> selectNode, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   std::vector<mlir::Attribute> attrs;
   std::vector<mlir::Attribute> names;
   std::vector<mlir::Attribute> colMemberNames;
   std::vector<mlir::Attribute> colTypes;
   auto& memberManager = builder.getContext()->getLoadedDialect<compiler::dialect::subop::SubOperatorDialect>()->getMemberManager();
   for (auto [name, columnInfo] : selectNode->targetInfo.namedResults) {
      names.push_back(builder.getStringAttr(name));
      auto colMemberName = memberManager.getUniqueMember(name.empty() ? "unnamed" : name);
      auto type = createTypeForColumn(builder.getContext(), columnInfo.column);
      colTypes.push_back(mlir::TypeAttr::get(type));
      colMemberNames.push_back(builder.getStringAttr(colMemberName));
      auto attrDef = attrManager.createRef(columnInfo.scope, columnInfo.column.getColumnName());
      attrs.push_back(attrDef);
   }

   mlir::Type localTableType = compiler::dialect::subop::LocalTableType::get(
      builder.getContext(),
      compiler::dialect::subop::StateMembersAttr::get(
         builder.getContext(),
         builder.getArrayAttr(colMemberNames),
         builder.getArrayAttr(colTypes)),
      builder.getArrayAttr(names));

   mlir::Value result = builder.create<compiler::dialect::relalg::MaterializeOp>(
      builder.getUnknownLoc(),
      localTableType,
      tree,
      builder.getArrayAttr(attrs),
      builder.getArrayAttr(names));

   // Use the materialized result in the QueryReturnOp instead of the input tree
   builder.create<compiler::dialect::relalg::QueryReturnOp>(builder.getUnknownLoc(), result);

   return std::make_pair(localTableType, result);
}
*/
template <class T>
std::pair<mlir::Type, mlir::Value> SQLMlirTranslator::translateTargetInfo(mlir::OpBuilder& builder, std::shared_ptr<T> selectNode, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   std::vector<mlir::Attribute> attrs;
   std::vector<mlir::Attribute> names;
   std::vector<mlir::Attribute> colMemberNames;
   std::vector<mlir::Attribute> colTypes;
   auto& memberManager = builder.getContext()->getLoadedDialect<compiler::dialect::subop::SubOperatorDialect>()->getMemberManager();
   for (auto& named : selectNode->targetInfo.namedResults) {
      auto name = named.first;
      if (named.second->type == ast::NamedResultType::Column) {
         auto columnInfo = std::static_pointer_cast<ast::ColumnInfo>(named.second);
         names.push_back(builder.getStringAttr(name));
         auto colMemberName = memberManager.getUniqueMember(name.empty() ? "unnamed" : name);
         auto type = createTypeForColumn(builder.getContext(), columnInfo->column);
         colTypes.push_back(mlir::TypeAttr::get(type));
         colMemberNames.push_back(builder.getStringAttr(colMemberName));
         auto attrDef = columnInfo->createRef(attrManager);
         attrs.push_back(attrDef);
      } else {
         auto functionInfo = std::static_pointer_cast<ast::FunctionInfo>(named.second);
         names.push_back(builder.getStringAttr(functionInfo->name));
         auto colMemberName = memberManager.getUniqueMember("max");
         colMemberNames.push_back(builder.getStringAttr(colMemberName));
         colTypes.push_back(mlir::TypeAttr::get(builder.getI64Type()));
         auto attrDef = functionInfo->createRef(attrManager);
         attrs.push_back(attrDef);
      }
   }

   mlir::Type localTableType = compiler::dialect::subop::LocalTableType::get(
      builder.getContext(),
      compiler::dialect::subop::StateMembersAttr::get(
         builder.getContext(),
         builder.getArrayAttr(colMemberNames),
         builder.getArrayAttr(colTypes)),
      builder.getArrayAttr(names));

   mlir::Value result = builder.create<compiler::dialect::relalg::MaterializeOp>(
      builder.getUnknownLoc(),
      localTableType,
      tree,
      builder.getArrayAttr(attrs),
      builder.getArrayAttr(names));

   // Use the materialized result in the QueryReturnOp instead of the input tree
   builder.create<compiler::dialect::relalg::QueryReturnOp>(builder.getUnknownLoc(), result);

   return std::make_pair(localTableType, result);
}

mlir::Value SQLMlirTranslator::translateWhereClause(mlir::OpBuilder& builder, std::shared_ptr<ast::ParsedExpression> whereClause, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   if (!whereClause) {
      return tree;
   }
   mlir::Block* pred = translatePredicate(builder, whereClause, context);
   auto sel = builder.create<compiler::dialect::relalg::SelectionOp>(builder.getUnknownLoc(), compiler::dialect::tuples::TupleStreamType::get(builder.getContext()), tree);
   sel.getPredicate().push_back(pred);
   tree = sel.getResult();
   return tree;
}

mlir::Block* SQLMlirTranslator::translatePredicate(mlir::OpBuilder& builder, std::shared_ptr<ast::ParsedExpression> predicate, std::shared_ptr<analyzer::SQLContext> context) {
   auto* block = new mlir::Block;
   mlir::OpBuilder predBuilder(builder.getContext());
   block->addArgument(compiler::dialect::tuples::TupleType::get(builder.getContext()), builder.getUnknownLoc());
   auto tupleScope = translationContext->createTupleScope();
   translationContext->setCurrentTuple(block->getArgument(0));

   predBuilder.setInsertionPointToStart(block);

   mlir::Value expr = translateExpression(predBuilder, predicate, context);
   predBuilder.create<compiler::dialect::tuples::ReturnOp>(builder.getUnknownLoc(), expr);
   return block;
}
/*
 * Expressions
 */
mlir::Value SQLMlirTranslator::translateExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ParsedExpression> expression, std::shared_ptr<analyzer::SQLContext> context) {
   if (expression->exprClass == ast::ExpressionClass::COMPARISON) {
      auto comparison = std::static_pointer_cast<ast::ComparisonExpression>(expression);
      return translateComparisonExpression(builder, comparison, context);
   }
   if (expression->exprClass == ast::ExpressionClass::CONJUNCTION) {
      auto conjunction = std::static_pointer_cast<ast::ConjunctionExpression>(expression);
      return translateConjunctionExpression(builder, conjunction, context);
   }
   if (expression->exprClass == ast::ExpressionClass::CONSTANT) {
      auto constantExpr = std::static_pointer_cast<ast::ConstantExpression>(expression);
      return translateConstExpression(builder, constantExpr, context);
   }
   if (expression->exprClass == ast::ExpressionClass::COLUMN_REF) {
      auto columnRefExpr = std::static_pointer_cast<ast::ColumnRefExpression>(expression);
      return translateColumnRefExpression(builder, columnRefExpr, context);
   }
   throw std::runtime_error("Not implemented");
}

mlir::Value SQLMlirTranslator::translateComparisonExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ComparisonExpression> expression, std::shared_ptr<analyzer::SQLContext> context) {
   //TODO Cast
   auto left = translateExpression(builder, expression->left, context);
   auto right = translateExpression(builder, expression->right, context);
   compiler::dialect::db::DBCmpPredicate pred;
   switch (expression->type) {
      case ast::ExpressionType::COMPARE_EQUAL:
         pred = compiler::dialect::db::DBCmpPredicate::eq;
         break;
      case ast::ExpressionType::COMPARE_NOTEQUAL:
         pred = compiler::dialect::db::DBCmpPredicate::neq;
         break;
      case ast::ExpressionType::COMPARE_LESSTHAN:
         pred = compiler::dialect::db::DBCmpPredicate::lt;
         break;
      case ast::ExpressionType::COMPARE_GREATERTHAN:
         pred = compiler::dialect::db::DBCmpPredicate::gt;
         break;
      case ast::ExpressionType::COMPARE_LESSTHANOREQUALTO:
         pred = compiler::dialect::db::DBCmpPredicate::lte;
         break;
      case ast::ExpressionType::COMPARE_GREATERTHANOREQUALTO:
         pred = compiler::dialect::db::DBCmpPredicate::gte;
         break;
      default: throw std::runtime_error("not implemented");
   }
   auto ct = compiler::frontend::sql::SQLTypeInference::toCommonBaseTypes(builder, {left, right});
   return builder.create<compiler::dialect::db::CmpOp>(builder.getUnknownLoc(), pred, ct[0], ct[1]);
}

mlir::Value SQLMlirTranslator::translateConjunctionExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ConjunctionExpression> conjunction, std::shared_ptr<analyzer::SQLContext> context) {
   std::vector<mlir::Value> values;
   for (auto exp : conjunction->children) {
      values.push_back(translateExpression(builder, exp, context));
   }
   switch (conjunction->type) {
      case ast::ExpressionType::CONJUNCTION_AND: {
         return builder.create<compiler::dialect::db::AndOp>(builder.getUnknownLoc(), values);
      }
      case ast::ExpressionType::CONJUNCTION_OR: {
         return builder.create<compiler::dialect::db::OrOp>(builder.getUnknownLoc(), values);
      }
      default: throw std::runtime_error("not implemented");
   }
}

mlir::Value SQLMlirTranslator::translateConstExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ConstantExpression> constatExpr, std::shared_ptr<analyzer::SQLContext> context) {
   if (constatExpr->value->type == ast::ConstantType::INT) {
      auto value = std::static_pointer_cast<ast::IntConstantValue>(constatExpr->value);
      return builder.create<compiler::dialect::db::ConstantOp>(builder.getUnknownLoc(), builder.getI32Type(), builder.getI32IntegerAttr(value->iVal));
   }
   if (constatExpr->value->type == ast::ConstantType::STRING) {
      auto value = std::static_pointer_cast<ast::StringConstantValue>(constatExpr->value);
      mlir::Type stringType = compiler::dialect::db::StringType::get(builder.getContext());
      if (value->sVal.size() <= 8 && value->sVal.size() > 0) {
         stringType = compiler::dialect::db::CharType::get(builder.getContext(), value->sVal.size());
      }
      return builder.create<compiler::dialect::db::ConstantOp>(builder.getUnknownLoc(), stringType, builder.getStringAttr(value->sVal));
   }
   throw std::runtime_error("Not implemented");
}

mlir::Value SQLMlirTranslator::translateColumnRefExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ColumnRefExpression> columnRefExpr, std::shared_ptr<analyzer::SQLContext> context) {
   if (columnRefExpr->columns.size() != 1) {
      throw std::runtime_error("Not implemented");
   }
   auto column = columnRefExpr->columns[0];
   auto name = columnRefExpr->alias.empty() ? column.getColumnName() : columnRefExpr->alias;
   auto attrDef = attrManager.createRef(columnRefExpr->scope, name);
   return builder.create<compiler::dialect::tuples::GetColumnOp>(
      builder.getUnknownLoc(),
      createTypeForColumn(builder.getContext(), column), attrDef, translationContext->getCurrentTuple());
}

/*
 * PipeSQL
 */
mlir::Value SQLMlirTranslator::translatePipeOpNode(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeSelectNode> pipeSelectNode, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   if (pipeOperator->type == ast::PipeOperatorType::SELECT) {
      return tree;
   }
   if (pipeOperator->type == ast::PipeOperatorType::FROM) {
      throw std::runtime_error("From should can only be used at the beginning of a pipe operator");
   }
   if (pipeOperator->type == ast::PipeOperatorType::WHERE) {
      mlir::Block* pred = translatePredicate(builder, std::static_pointer_cast<ast::ParsedExpression>(pipeOperator->node), context);
      auto sel = builder.create<compiler::dialect::relalg::SelectionOp>(builder.getUnknownLoc(), compiler::dialect::tuples::TupleStreamType::get(builder.getContext()), tree);
      sel.getPredicate().push_back(pred);
      tree = sel.getResult();
      return tree;
   }
   if (pipeOperator->type == ast::PipeOperatorType::AGGREGATE) {
      return translateAggregatePipeOperator(builder, pipeOperator, context, tree);
   }
   if (pipeOperator->type == ast::PipeOperatorType::ORDER_BY) {
      return translateOrderByModifier(builder, std::static_pointer_cast<ast::OrderByModifier>(pipeOperator->node), context, tree);
   }

   throw std::runtime_error("Not implemented");
}

mlir::Value SQLMlirTranslator::translateAggregatePipeOperator(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   assert(pipeOperator->type == ast::PipeOperatorType::AGGREGATE);
   auto aggreationNode = std::static_pointer_cast<ast::AggregationNode>(pipeOperator->node);
   return translateGroupByNode(builder, aggreationNode->groupByNode, aggreationNode->aggregations, context, tree);
}

mlir::Value SQLMlirTranslator::translateGroupByNode(mlir::OpBuilder& builder, std::shared_ptr<ast::GroupByNode> groupByClause, std::vector<std::shared_ptr<ast::FunctionExpression>> aggregations, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   std::vector<mlir::Attribute> groupByAttrs;
   //Only Local GroupBy
   for (auto expr : groupByClause->group_expressions) {
      if (expr->type == ast::ExpressionType::COLUMN_REF) {
         auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(expr);
         assert(columnRef->columns.size() == 1);
         auto attrDef = attrManager.createRef(columnRef->scope, columnRef->columns[0].getColumnName());
         groupByAttrs.push_back(attrDef);
      } else {
         throw std::runtime_error("Not implemented");
      }
   }

   auto tupleType = compiler::dialect::tuples::TupleType::get(builder.getContext());
   auto tupleStreamType = compiler::dialect::tuples::TupleStreamType::get(builder.getContext());
   std::string groupByName = "aggr" + std::to_string(1);
   auto tupleScope = translationContext->createTupleScope();
   auto* block = new mlir::Block;
   block->addArgument(tupleStreamType, builder.getUnknownLoc());
   block->addArgument(tupleType, builder.getUnknownLoc());
   mlir::Value relation = block->getArgument(0);
   mlir::OpBuilder aggrBuilder(builder.getContext());
   aggrBuilder.setInsertionPointToStart(block);
   std::vector<mlir::Value> createdValues;
   std::vector<mlir::Attribute> createdCols;

   for (auto expr : aggregations) {
      mlir::Value translatedExpr;
      if (expr->type == ast::ExpressionType::AGGREGATE && expr->exprClass == ast::ExpressionClass::FUNCTION) {
         auto function = std::static_pointer_cast<ast::FunctionExpression>(expr);

         std::string aggrFuncName = function->functionName;
         auto aggrFunc = llvm::StringSwitch<compiler::dialect::relalg::AggrFunc>(aggrFuncName)
                            .Case("sum", compiler::dialect::relalg::AggrFunc::sum)
                            .Case("avg", compiler::dialect::relalg::AggrFunc::avg)
                            .Case("min", compiler::dialect::relalg::AggrFunc::min)
                            .Case("max", compiler::dialect::relalg::AggrFunc::max)
                            .Case("count", compiler::dialect::relalg::AggrFunc::count)
                            .Case("stddev_samp", compiler::dialect::relalg::AggrFunc::stddev_samp)
                            .Default(compiler::dialect::relalg::AggrFunc::count);

         compiler::dialect::tuples::ColumnRefAttr refAttr;
         mlir::Type aggrResultType = builder.getI64Type();
         mlir::Value currRel = relation;
         if (expr->arguments.size() != 1 || expr->arguments[0]->type != ast::ExpressionType::COLUMN_REF) {
            throw("Arguments not supported");
         }
         auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(expr->arguments[0]);
         assert(columnRef->columns.size() == 1);
         refAttr = attrManager.createRef(columnRef->scope, columnRef->columns[0].getColumnName());
         translatedExpr = aggrBuilder.create<compiler::dialect::relalg::AggrFuncOp>(builder.getUnknownLoc(), aggrResultType, aggrFunc, currRel, refAttr);
         auto attrDef = attrManager.createDef(function->scope, function->alias.empty() ? function->functionName : function->alias);

         attrDef.getColumn().type = translatedExpr.getType();
         //  mapping.insert({toAggr.first->colId, &attrDef.getColumn()});
         createdCols.push_back(attrDef);
         createdValues.push_back(translatedExpr);
      }
   }

   aggrBuilder.create<compiler::dialect::tuples::ReturnOp>(builder.getUnknownLoc(), createdValues);
   auto groupByOp = builder.create<compiler::dialect::relalg::AggregationOp>(builder.getUnknownLoc(), tupleStreamType, tree, builder.getArrayAttr(groupByAttrs), builder.getArrayAttr(createdCols));
   groupByOp.getAggrFunc().push_back(block);
   return groupByOp.getResult();
}

/*
 * Resultmodifier
*/
mlir::Value SQLMlirTranslator::translateModifier(mlir::OpBuilder& builder, std::shared_ptr<ast::ResultModifier> resultModifier, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   if (resultModifier->modifierType == ast::ResultModifierType::ORDER_BY) {
      return translateOrderByModifier(builder, std::static_pointer_cast<ast::OrderByModifier>(resultModifier), context, tree);
   }
   if (resultModifier->modifierType == ast::ResultModifierType::LIMIT) {
      return translateLimitModifier(builder, std::static_pointer_cast<ast::LimitModifier>(resultModifier), context, tree);
   }
   throw std::runtime_error("Not implemented");
}
mlir::Value SQLMlirTranslator::translateOrderByModifier(mlir::OpBuilder& builder, std::shared_ptr<ast::OrderByModifier> orderByModifier, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   //TODO

   std::vector<mlir::Attribute> mapping;
   for (auto orderByElement : orderByModifier->orderByElements) {
      compiler::dialect::relalg::SortSpec spec = compiler::dialect::relalg::SortSpec::asc;
      if (orderByElement->type == ast::OrderType::DESCENDING) {
         spec = compiler::dialect::relalg::SortSpec::desc;
      }
      if (orderByElement->expression->type == ast::ExpressionType::COLUMN_REF) {
         auto columnRef = std::static_pointer_cast<ast::ColumnRefExpression>(orderByElement->expression);
         assert(columnRef->columns.size() == 1);
         auto attrDef = attrManager.createRef(columnRef->scope, columnRef->columns[0].getColumnName());
         mapping.push_back(compiler::dialect::relalg::SortSpecificationAttr::get(builder.getContext(), attrDef, spec));
      } else {
         throw std::runtime_error("Not implemented");
      }
   }
   return builder.create<compiler::dialect::relalg::SortOp>(builder.getUnknownLoc(), compiler::dialect::tuples::TupleStreamType::get(builder.getContext()), tree, builder.getArrayAttr(mapping));
   ;
}

mlir::Value SQLMlirTranslator::translateLimitModifier(mlir::OpBuilder& builder, std::shared_ptr<ast::LimitModifier> limitModifier, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree) {
   auto value = std::static_pointer_cast<ast::IntConstantValue>(std::static_pointer_cast<ast::ConstantExpression>(limitModifier->limitExpression)->value);
   return builder.create<compiler::dialect::relalg::LimitOp>(builder.getUnknownLoc(), compiler::dialect::tuples::TupleStreamType::get(builder.getContext()), value->iVal, tree);
}

/*
 * Helper functions
 */

mlir::Type SQLMlirTranslator::createTypeForColumn(mlir::MLIRContext* context, const catalog::Column& colDef) {
   mlir::Type baseType = createBaseTypeFromColumnType(context, colDef.getLogicalType());
   return colDef.getIsNullable() ? compiler::dialect::db::NullableType::get(context, baseType) : baseType;
}

mlir::Type SQLMlirTranslator::createBaseTypeFromColumnType(mlir::MLIRContext* context, const catalog::Type& t) {
   return t.getMLIRTypeCreator()->createType(context);
}

} // namespace lingodb::translator