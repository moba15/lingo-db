#ifndef LINGODB_COMPILER_FRONTEND_PL_DRIVER_H
#define LINGODB_COMPILER_FRONTEND_PL_DRIVER_H

#include "lingodb/compiler/frontend/pl/pl_parser.hpp"
#include "lingodb/compiler/frontend/driver.h" // To use the SQL parser driver
#include "lingodb/compiler/frontend/node_factory.h"
#include "lingodb/compiler/frontend/pl/pl_ast.h"

// This is the driver for the PL/pgSQL parser. It's similar to the main SQL driver.
// It orchestrates the lexer and parser.

#define YY_DECL \
   lingodb::frontend::pl::parser::symbol_type plpgsql_yylex(pl_driver& drv)

YY_DECL;

class pl_driver {
public:
   pl_driver(const std::string& text) : text(text), traceScanning(false), traceParsing(false) {};
   ~pl_driver() = default;

   // The result of parsing the PL/pgSQL code will be stored here.
   std::shared_ptr<lingodb::ast::pl::Block> result;
   const std::string& text;

   int parse();
   void scanBegin();
   void scanEnd();

   lingodb::location location;
   bool traceScanning;
   bool traceParsing;
};

#endif // LINGODB_COMPILER_FRONTEND_PL_DRIVER_H