#pragma once
#include "lingodb/compiler/Dialect/TupleStream/ColumnManager.h"
#include "lingodb/compiler/frontend/SQL/Parser.h"
#include "lingodb/compiler/frontend/analyzer/sql_context.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/comparison_expression.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/conjunction_expression.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/constant_expression.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/function_expression.h"
#include "lingodb/compiler/frontend/sql-parser/query_node/pipe_select_node.h"
#include "lingodb/compiler/frontend/sql-parser/query_node/select_node.h"
#include "lingodb/compiler/frontend/sql-parser/tableref/basetableref.h"
#include "lingodb/compiler/frontend/sql-parser/tableref/joinref.h"
#include "lingodb/compiler/frontend/translator/context/translation_context.h"

#include <memory>

#include "mlir/IR/BuiltinOps.h"

namespace lingodb::translator {
class SQLMlirTranslator {
   public:
   SQLMlirTranslator(mlir::ModuleOp moduleOp, std::shared_ptr<catalog::Catalog> catalog);
   mlir::ModuleOp moduleOp;
   compiler::dialect::tuples::ColumnManager& attrManager;

   std::optional<mlir::Value> translate(mlir::OpBuilder& builder, std::shared_ptr<ast::QueryNode> queryNode, std::shared_ptr<analyzer::SQLContext> context);

   private:
   std::shared_ptr<TranslationContext> translationContext;

   std::shared_ptr<catalog::Catalog> catalog;
   mlir::Type translateSelectNode(mlir::OpBuilder& builder, std::shared_ptr<ast::SelectNode> selectNode, std::shared_ptr<analyzer::SQLContext> context, mlir::Block* block);
   mlir::Value translateSubSelectNode(mlir::OpBuilder& builder, std::shared_ptr<ast::SelectNode> selectNode, std::shared_ptr<analyzer::SQLContext> context);

   std::pair<mlir::Value, mlir::Type> translatePipeSelectNode(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeSelectNode> pipeSelectNode, std::shared_ptr<analyzer::SQLContext> context, mlir::Block* block);
   mlir::Value translateSubPipeSelectNode(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeSelectNode> pipeSelectNode, std::shared_ptr<analyzer::SQLContext> context);
   mlir::Value translatePipeOpNode(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeSelectNode> pipeSelectNode, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);

   /*
    * TableRef
    */
   mlir::Value translateTableRef(mlir::OpBuilder& builder, std::shared_ptr<ast::TableRef> fromClause, std::shared_ptr<analyzer::SQLContext> context);
   mlir::Value translateBaseTableRef(mlir::OpBuilder& builder, std::shared_ptr<ast::BaseTableRef> baseTableRef, std::shared_ptr<analyzer::SQLContext> context);
   mlir::Value translateJoinTableRef(mlir::OpBuilder& builder, std::shared_ptr<ast::JoinRef> join, std::shared_ptr<analyzer::SQLContext> context);

   //std::pair<mlir::Type, mlir::Value> translateTargetSelectionList(mlir::OpBuilder& builder, std::shared_ptr<ast::SelectNode> selectNode, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);
   template <class T>
   std::pair<mlir::Type, mlir::Value> translateTargetInfo(mlir::OpBuilder& builder, std::shared_ptr<T> selectNode, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);

   mlir::Value translateAggregatePipeOperator(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);

   mlir::Value translateGroupByNode(mlir::OpBuilder& builder, std::shared_ptr<ast::GroupByNode> groupByClause, std::vector<std::shared_ptr<ast::FunctionExpression>> aggregation, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);

   mlir::Value translateWhereClause(mlir::OpBuilder& builder, std::shared_ptr<ast::ParsedExpression> whereClause, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);
   mlir::Block* translatePredicate(mlir::OpBuilder& builder, std::shared_ptr<ast::ParsedExpression> predicate, std::shared_ptr<analyzer::SQLContext> context);

   /*
    * Expressions
    */
   mlir::Value translateExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ParsedExpression> expression, std::shared_ptr<analyzer::SQLContext> context);
   mlir::Value translateComparisonExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ComparisonExpression> expression, std::shared_ptr<analyzer::SQLContext> context);
   mlir::Value translateConjunctionExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ConjunctionExpression> conjunction, std::shared_ptr<analyzer::SQLContext> context);
   mlir::Value translateConstExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ConstantExpression> constatExpr, std::shared_ptr<analyzer::SQLContext> context);
   mlir::Value translateColumnRefExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::ColumnRefExpression> columnRefExpr, std::shared_ptr<analyzer::SQLContext> context);

   /*
    * Resultmodifier
    */
   mlir::Value translateModifier(mlir::OpBuilder& builder, std::shared_ptr<ast::ResultModifier> resultModifier, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);
   mlir::Value translateOrderByModifier(mlir::OpBuilder& builder, std::shared_ptr<ast::OrderByModifier> orderByModifier, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);
   mlir::Value translateLimitModifier(mlir::OpBuilder& builder, std::shared_ptr<ast::LimitModifier> limitModifier, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);

   public:
   mlir::Type createTypeForColumn(mlir::MLIRContext* context, const catalog::Column& colDef);
   mlir::Type createBaseTypeFromColumnType(mlir::MLIRContext* context, const catalog::Type& t);
};
} // namespace lingodb::translator