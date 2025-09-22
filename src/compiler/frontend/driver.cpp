#include "lingodb/compiler/frontend/driver.h"

driver::driver()
   : traceScanning(false) {}

int driver::parse(const std::string& f) {
   file = f;
   location.initialize(&file);
   scanBegin();
   lingodb::parser parse(*this);
   int res = parse();
   scanEnd();
   return res;
}