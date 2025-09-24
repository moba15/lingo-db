#ifndef TMP_CLION_CLANG_TIDY_PL_DRIVER_H
#define TMP_CLION_CLANG_TIDY_PL_DRIVER_H

#pragma once

#include "lingodb/compiler/plpgsql-frontend/sql-parser/gen/parser.hpp"

#define YY_DECL \
   lingodb::frontend::pl::parser::symbol_type yylex(pl_driver& drv)
// ... and declare it for the parser's sake.
YY_DECL;

class pl_driver {
   public:
   pl_driver();
   ~pl_driver() {
   };
   int parse(const std::string& f);
   void scan_begin();
   void scan_end();
   std::string file;
   bool trace_scanning;
   bool trace_parsing;
   lingodb::frontend::pl::location location;
};
#endif
