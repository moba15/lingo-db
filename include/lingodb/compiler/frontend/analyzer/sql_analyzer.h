#pragma once
#include "lingodb/compiler/frontend/sql-parser/driver.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
#include "sql_context.h"

#include <functional>
namespace lingodb::analyzer {
class Analyzer;
class SQLQueryAnalyzer {
   public:
   SQLQueryAnalyzer(std::shared_ptr<catalog::Catalog> catalog);
   std::shared_ptr<SQLContext> context = std::make_shared<SQLContext>();

   std::shared_ptr<ast::TableProducer> analyze(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context);

   private:
   std::shared_ptr<catalog::Catalog> catalog;
   driver drv{};

   void analyzeRootNode(std::shared_ptr<ast::AstNode> rootNode);
   std::shared_ptr<ast::TableProducer> transform(std::shared_ptr<ast::TableProducer> rootNode, std::shared_ptr<SQLContext> context);

   private:
   std::shared_ptr<ast::AggregationNode> aggregation = std::make_shared<ast::AggregationNode>();

   void transformTargetSelections(std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context);
   void transformGroupByClause(std::shared_ptr<ast::SelectNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context);
   template <class T>
   void analyzePipeOperator(std::shared_ptr<T> rootNode, std::shared_ptr<ast::PipeOperator> pipeOperator, std::shared_ptr<SQLContext> context);
   template <class T>
   void analyzeTargetSelections(std::shared_ptr<T> rootSelectNode, std::shared_ptr<ast::TargetsExpression> targetSelection, std::shared_ptr<SQLContext> context);

   void analyzeFromClause(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::TableRef> tableRef, std::shared_ptr<SQLContext> context);
   void analyzeWhereClause(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::ParsedExpression> whereClause, std::shared_ptr<SQLContext> context);

   void analyzeAggregation(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::AggregationNode> aggregationNode, std::shared_ptr<SQLContext> context);
   void analyzeGroupByNode(std::shared_ptr<ast::QueryNode> rootSelectNode, std::shared_ptr<ast::GroupByNode> groupByNode, std::shared_ptr<SQLContext> context);

   void analyzeModifiers(std::shared_ptr<ast::SelectNode> rootNode, std::vector<std::shared_ptr<ast::ResultModifier>> modifiers, std::shared_ptr<SQLContext> context);
   void analyzeOrderByModifier(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::OrderByModifier> orderByModifier, std::shared_ptr<SQLContext> context);
   void analyzeLimitModifier(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<ast::LimitModifier> limitModifier, std::shared_ptr<SQLContext> context);

   ///Expressions
   void analyzeExpression(std::shared_ptr<ast::ParsedExpression> rootNode, std::shared_ptr<SQLContext> context);
   void analyzeComparisonExpression(std::shared_ptr<ast::ComparisonExpression> comparison, std::shared_ptr<SQLContext> context);
   void analyzeColumnRefExpression(std::shared_ptr<ast::ColumnRefExpression> columnRef, std::shared_ptr<SQLContext> context);
   void analyzeStarRefExpression(const std::shared_ptr<ast::StarExpression> star, const std::shared_ptr<SQLContext> shared);
   void analyzeConstExpression(std::shared_ptr<ast::ConstantExpression> constExpr, std::shared_ptr<SQLContext> context);
   void analyzeConjunctionExpression(std::shared_ptr<ast::ConjunctionExpression> conjunction, std::shared_ptr<SQLContext> context);
   void analyzeAggregationFunctionExpression(std::shared_ptr<ast::FunctionExpression> function, std::shared_ptr<SQLContext> context);

   std::string createTmpScope() {
      static size_t tmpScopeCounter = 0;
      std::string scope{"tmp_attr"};
      scope.append(std::to_string(tmpScopeCounter++));
      return scope;
   }

   void error(std::string message, location loc) {
      std::ostringstream s{};
      s << message << " at " << loc << std::endl;
      throw std::runtime_error(s.str());
   }
};

} // namespace lingodb::analyzer