#include "lingodb/compiler/frontend/analyzer/sql_analyzer.h"

#include "lingodb/compiler/frontend/sql-parser/driver.h"
#include <iostream>
#include <lingodb/runtime/Session.h>

int main(int argc, char* argv[]) {
   if (argc <= 2) {
      std::cerr << "USAGE: <sql database> <sql statement>" << std::endl;
      return 1;
   }

   auto session = lingodb::runtime::Session::createSession(std::string(argv[1]), true);
   driver drv;
   if (!drv.parse(argv[2])) {
      if (!drv.result) {
         std::cerr << "Error parsing query" << std::endl;
         return 1;
      }
      std::cout << "digraph ast {" << std::endl;
      lingodb::ast::NodeIdGenerator idGen{};
      std::cout << drv.result->toDotGraph(1, idGen) << std::endl;
      std::cout << "}" << std::endl;

      lingodb::analyzer::SQLQueryAnalyzer sqlAnalyzer{session->getCatalog()};
      auto context = std::make_shared<lingodb::analyzer::SQLContext>();
      context->catalog = session->getCatalog();
      lingodb::analyzer::SQLQueryAnalyzer analyzer{session->getCatalog()};
      drv.result = analyzer.analyzeAndTransform(drv.result, context);
      std::cout << std::endl
                << std::endl;
      std::cout << "After" << std::endl;
      std::cout << "digraph ast {" << std::endl;
      std::cout << drv.result->toDotGraph(1, idGen) << std::endl;
      std::cout << "}" << std::endl;

      std::cout << "Context:" << std::endl;
      std::cout << analyzer.context->toString() << std::endl;

   } else {
      return 1;
   }

   return 0;
} // namespace lingodb