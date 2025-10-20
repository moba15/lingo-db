#include "lingodb/compiler/frontend/pl/pl_driver.h"

#include "lingodb/compiler/frontend/generated/pl/pl_parser.hpp"


#include <lingodb/compiler/frontend/UDFImplementer.h>

pl_driver::pl_driver()
   : trace_parsing(false), trace_scanning(false) {}

int pl_driver::parse(const std::string& f) {
   file = f;

   scan_begin();
   lingodb::frontend::pl::parser parse(*this);
   parse.set_debug_level(trace_parsing);
   int res = parse();
   scan_end();
   return res;
}
