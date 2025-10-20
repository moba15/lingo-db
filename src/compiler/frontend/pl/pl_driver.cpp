#include "lingodb/compiler/frontend/pl/pl_driver.h"
#include "lingodb/compiler/frontend/pl/pl_parser.hpp"

// These functions are implemented in the lexer (pl_lexer.l)
extern void pl_scan_string(const char* s);
extern void pl_delete_buffer();

int pl_driver::parse() {
   scanBegin();
   lingodb::frontend::pl::parser parse(*this);
   parse.set_debug_level(traceParsing);
   int res = parse();
   scanEnd();
   return res;
}

void pl_driver::scanBegin() {
   // yy_flex_debug is a global from flex
   extern int plpgsql_debug;
   plpgsql_debug = traceScanning;
   pl_scan_string(text.c_str());
}

void pl_driver::scanEnd() {
   pl_delete_buffer();
}