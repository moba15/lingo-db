#include "lingodb/compiler/plpgsql-frontend/pl_driver.h"

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