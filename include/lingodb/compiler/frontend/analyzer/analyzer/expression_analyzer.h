#pragma once
#include "lingodb/compiler/frontend/analyzer/sql_analyzer.h"
#include "lingodb/compiler/frontend/sql-parser/ast_node.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/list.h"
namespace lingodb::analyzer {
class ExpressionAnalyzer : public Analyzer {
   public:
   ExpressionAnalyzer() = default;
   void analyze(std::shared_ptr<ast::AstNode> rootNode, std::shared_ptr<SQLContext> context) override;
   void error(std::string message, lingodb::location loc) override;

   private:
   void analyzeComparisonExpression(std::shared_ptr<ast::ComparisonExpression> comparison, std::shared_ptr<SQLContext> context);
   void analyzeColumnRefExpression(std::shared_ptr<ast::ColumnRefExpression> columnRef, std::shared_ptr<SQLContext> context);
   void analyzeConstExpression(std::shared_ptr<ast::ConstantExpression> constExpr, std::shared_ptr<SQLContext> context);
   void analyzeConjunctionExpression(std::shared_ptr<ast::ConjunctionExpression> conjunction, std::shared_ptr<SQLContext> context);
};
} // namespace lingodb::analyzer