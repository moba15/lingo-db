#pragma once
#include "lingodb/compiler/frontend/analyzer/analyzer/select_node_analyzer.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
#include "sql_context.h"

#include <functional>
namespace lingodb::analyzer {
class Analyzer;
class SQLQueryAnalyzer {
   public:
   SQLQueryAnalyzer(std::shared_ptr<catalog::Catalog> catalog);
   std::shared_ptr<SQLContext> context = std::make_shared<SQLContext>();

   void analyze(std::shared_ptr<ast::AstNode> rootNode);

   private:
   std::shared_ptr<catalog::Catalog> catalog;

   void analyzeRootNode(std::shared_ptr<ast::AstNode> rootNode);

   void error(std::string message, location loc);
};

} // namespace lingodb::analyzer