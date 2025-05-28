#include "lingodb/compiler/frontend/sql-parser/driver.h"
#include <iostream>

int main(int argc, char* argv[]) {
   int res = 0;
   std::string test;

   driver drv;
   for (int i = 1; i < argc; ++i)
      if (argv[i] == std::string("-p"))
         drv.trace_parsing = true;
      else if (argv[i] == std::string("-s"))
         drv.trace_scanning = true;
      else if (!drv.parse(argv[i])) {
         std::cout << "digraph ast {" << std::endl;
         lingodb::ast::NodeIdGenerator idGen{};
         std::cout << drv.result->toDotGraph(1, idGen) << std::endl;
         std::cout << "}" << std::endl;
         std::cout << std::endl;
         std::cout << std::endl;

      } else
         res = 1;
   return res;
}