#include "lingodb/compiler/frontend/driver.h"

Driver::Driver()
   : traceScanning(false) {}

int Driver::parse(const std::string& f) {
   file = f;
   location.initialize(&file);
   scanBegin();
   lingodb::parser parse(*this);
   int res = parse();
   scanEnd();
   return res;
}