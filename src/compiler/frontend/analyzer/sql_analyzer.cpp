#include "lingodb/compiler/frontend/analyzer/sql_analyzer.h"
namespace lingodb::analyzer {
SQLQueryAnalyzer::SQLQueryAnalyzer(std::shared_ptr<catalog::Catalog> catalog) : catalog(std::move(catalog)) {
}
void SQLQueryAnalyzer::analyze(std::shared_ptr<ast::AstNode> rootNode) {
   analyzeRootNode(rootNode);
   SelectNodeAnalyzer selectNodeAnalyzer{catalog};
   //Analyze Select node
   if (rootNode->nodeType == ast::NodeType::QUERY_NODE) {
      auto queryNode = std::static_pointer_cast<ast::QueryNode>(rootNode);
      if (queryNode->type == ast::QueryNodeType::SELECT_NODE || queryNode->type == ast::QueryNodeType::PIPE_NODE) {
         selectNodeAnalyzer.analyze(rootNode, context);
      }
   }
}
void SQLQueryAnalyzer::analyzeRootNode(std::shared_ptr<ast::AstNode> rootNode) {
   if (rootNode->nodeType != ast::NodeType::QUERY_NODE && rootNode->nodeType != ast::NodeType::PIPE_OP) {
      error("Query must start with an Select Query or FROM", rootNode->loc);
   }
}

void SQLQueryAnalyzer::error(std::string message, location loc) {
   std::cerr << "Error" << message << loc << std::endl;
   std::cerr << "Context: " << std::endl;
}
} // namespace lingodb::analyzer