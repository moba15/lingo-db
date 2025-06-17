#pragma once
#include "lingodb/compiler/Dialect/TupleStream/ColumnManager.h"
#include "lingodb/compiler/frontend/SQL/Parser.h"
#include "lingodb/compiler/frontend/analyzer/bound/bound_aggregation.h"
#include "lingodb/compiler/frontend/analyzer/bound/bound_resultmodifier.h"
#include "lingodb/compiler/frontend/analyzer/bound/bound_tableref.h"
#include "lingodb/compiler/frontend/analyzer/sql_context.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include "lingodb/compiler/frontend/sql-parser/select_node.h"
#include "lingodb/compiler/frontend/translator/context/translation_context.h"

#include <memory>

#include "mlir/IR/BuiltinOps.h"

namespace lingodb::translator {
#define error(message, loc)                                         \
   {                                                                \
      std::ostringstream s{};                                       \
      s << "Translator: " << message << " at " << loc << std::endl; \
      throw std::runtime_error(s.str());                            \
   }
class SQLMlirTranslator {
   public:
   SQLMlirTranslator(mlir::ModuleOp moduleOp, std::shared_ptr<catalog::Catalog> catalog);
   mlir::ModuleOp moduleOp;
   compiler::dialect::tuples::ColumnManager& attrManager;

   std::optional<mlir::Value> translateStart(mlir::OpBuilder& builder, std::shared_ptr<ast::TableProducer> tableProducer, std::shared_ptr<analyzer::SQLContext> context);

   private:
   std::shared_ptr<TranslationContext> translationContext;
   std::shared_ptr<catalog::Catalog> catalog;

   mlir::Value translateTableProducer(mlir::OpBuilder& builder, std::shared_ptr<ast::TableProducer> tableProducer, std::shared_ptr<analyzer::SQLContext> context);

   mlir::Value translatePipeOperator(mlir::OpBuilder& builder, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);

   mlir::Value translateResultModifier(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundResultModifier> resultModifier, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);

   mlir::Value translateExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundExpression> expression, std::shared_ptr<analyzer::SQLContext> context);
   mlir::Value translateBinaryOperatorExpression(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundOperatorExpression> expression, std::shared_ptr<analyzer::SQLContext> context, mlir::Value left, mlir::Value right);

   mlir::Value translateTableRef(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundTableRef> tableRef, std::shared_ptr<analyzer::SQLContext> context);

   mlir::Value translateAggregation(mlir::OpBuilder& builder, std::shared_ptr<ast::BoundAggregationNode> aggregation, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);
   mlir::Value createMap(mlir::OpBuilder& builder, std::string mapName, std::vector<std::shared_ptr<ast::BoundExpression>> toMap, std::shared_ptr<analyzer::SQLContext> context, mlir::Value tree);

   /*
    * Utility
   */

   public:
   /*
   * Helper functions
   */
   mlir::Type createTypeForColumn(mlir::MLIRContext* context, const catalog::Column& colDef);
   mlir::Type createBaseTypeFromColumnType(mlir::MLIRContext* context, const catalog::Type& t);
};
} // namespace lingodb::translator