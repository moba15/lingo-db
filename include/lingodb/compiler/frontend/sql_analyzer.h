#pragma once
#include "lingodb/compiler/frontend/ast/bound/bound_expression.h"
#include "lingodb/compiler/frontend/ast/bound/bound_resultmodifier.h"
#include "lingodb/compiler/frontend/driver.h"
#include "lingodb/compiler/frontend/ast/query_node.h"
#include "sql_context.h"

#include <functional>
namespace lingodb::analyzer {
using ResolverScope = llvm::ScopedHashTable<std::string, std::shared_ptr<ast::NamedResult>, StringInfo>::ScopeTy;
#define error(message, loc)                       \
   {                                              \
      std::ostringstream s{};                     \
      s << message << " at " << loc << std::endl; \
      throw std::runtime_error(s.str());          \
   }

class Analyzer;
class SQLQueryAnalyzer {
   public:
   SQLQueryAnalyzer(std::shared_ptr<catalog::Catalog> catalog);
   std::shared_ptr<SQLContext> context = std::make_shared<SQLContext>();

   std::shared_ptr<ast::TableProducer> analyzeAndTransform(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context);
   std::shared_ptr<ast::TableProducer> analyze(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context, ResolverScope& resolverScope);
   std::shared_ptr<ast::TableProducer> transform(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<ASTTransformContext> context);
   std::shared_ptr<ast::ParsedExpression> transformParsedExpression(std::shared_ptr<ast::ParsedExpression> rootNode, std::shared_ptr<ASTTransformContext> context);

   private:
   std::shared_ptr<catalog::Catalog> catalog;
   driver drv{};

   template <class T>
   std::shared_ptr<T> transformCast(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<ASTTransformContext> context);

   private:
   std::shared_ptr<ast::TableProducer> analyzePipeOperator(std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<SQLContext> context, ResolverScope& resolverScope);
   std::shared_ptr<ast::TableProducer> analyzeTableRef(std::shared_ptr<ast::TableRef> tableRef, std::shared_ptr<SQLContext> context, ResolverScope& resolverScope);
   std::shared_ptr<ast::BoundResultModifier> analyzeResultModifier(std::shared_ptr<ast::ResultModifier> resultModifier, std::shared_ptr<SQLContext> context);

   ///Expressions
   std::shared_ptr<ast::BoundExpression> analyzeExpression(std::shared_ptr<ast::ParsedExpression> rootNode, std::shared_ptr<SQLContext> context, ResolverScope& resolverScope);
   std::shared_ptr<ast::BoundColumnRefExpression> analyzeColumnRefExpression(std::shared_ptr<ast::ColumnRefExpression> columnRef, std::shared_ptr<SQLContext> context);

   catalog::NullableType getCommonType(catalog::NullableType nullableType1, catalog::NullableType nullableType2);
   [[nodiscard]]
   std::pair<unsigned long, unsigned long> getAdaptedDecimalPAndSAfterMulDiv(unsigned long p, unsigned long s);
   catalog::NullableType getCommonTypeAfterOperation(catalog::NullableType type1, catalog::NullableType type2, ast::ExpressionType operationType);
   catalog::NullableType getCommonBaseType(std::vector<catalog::NullableType> types, ast::ExpressionType operationType);
   catalog::NullableType getHigherDecimalType(catalog::NullableType left, catalog::NullableType right);

   catalog::NullableType getCommonBaseType(std::vector<catalog::NullableType> types);
   std::vector<catalog::NullableType> toCommonNumber(std::vector<catalog::NullableType> types);
   std::vector<catalog::NullableType> toCommonTypes(std::vector<catalog::NullableType> types);
   std::string createTmpScope() {
      static size_t tmpScopeCounter = 0;
      std::string scope{"tmp_attr"};
      scope.append(std::to_string(tmpScopeCounter++));
      return scope;
   }

   std::string createMapName() {
      static size_t mapCounter = 0;
      std::string mapName{"map"};
      mapName.append(std::to_string(mapCounter++));
      return mapName;
   }


};

} // namespace lingodb::analyzer