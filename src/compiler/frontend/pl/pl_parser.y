%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.2"

%define api.token.raw
%define api.namespace    { lingodb::frontend::pl }
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.prefix {plpgsql_}


%code requires {
  # include <string>
  #include <iostream>
  #include <memory>
  #include <vector>
  #include <utility>
  #include <sstream>
  #include <variant>
  class pl_driver;
  /*
   * The idea is to have a separate AST for the PL/pgSQL language.
   * This AST will embed the standard SQL AST for SQL statements.
  */
  #include "lingodb/compiler/frontend/pl/pl_ast.h"
  /*
   * We include the main SQL driver to be able to invoke the SQL parser
   * from within our PL/pgSQL parser's actions.
  */
  #include "lingodb/compiler/frontend/driver.h"
}

// The parsing context.
%param { pl_driver& drv }



%define parse.trace
%define parse.error detailed
%define parse.lac full
%locations

%code {
  #include <iostream>
  #include <memory>
  #include "lingodb/compiler/frontend/pl/pl_driver.h"

}

%define api.token.prefix {TOK_}
/*
 * This is the new token that will hold the raw SQL string
 * captured by the lexer.
*/
%token <std::string> SQL_TEXT "SQL text"
%token DECLARE BEGIN END RETURN_P SEMICOLON


%type <std::shared_ptr<lingodb::ast::pl::Block>> pl_block
%type <std::vector<std::shared_ptr<lingodb::ast::pl::Stmt>>> proc_sect
%type <std::shared_ptr<lingodb::ast::pl::Stmt>> proc_stmt stmt_return

%printer { yyo << $$; } <*>;

%%
%start pl_function;


pl_function:
  pl_block { drv.result = $1; }
;



pl_block:
  decl_sect BEGIN proc_sect END SEMICOLON
  {
      auto block = std::make_shared<lingodb::ast::pl::Block>();
      block->body = $3;
      $$ = block;
  }
;

decl_sect:
  opt_block_label
  | opt_block_label decl_start
  ;

opt_block_label:
  %empty
  ;
decl_start:
  DECLARE
  ;


proc_sect:
  proc_sect[list] proc_stmt
  {
      $list.push_back($2);
      $$ = $list;
  }
  | %empty
  {
      $$ = std::vector<std::shared_ptr<lingodb::ast::pl::Stmt>>();
  }
;
//TODO
proc_stmt:
  pl_block SEMICOLON { $$ = $1; }
  | stmt_return { $$ = $1; }
;

    stmt_return:
      RETURN_P SQL_TEXT SEMICOLON
      {
          /*
           * This is the core of the solution.
           * 1. The lexer has provided the SQL statement as a raw string in SQL_TEXT ($2).
           * 2. We create an instance of the standard SQL parser driver.
           * 3. We call its parse() method on the captured string.
           * 4. The resulting SQL AST is retrieved from the driver.
           * 5. We create a PL/pgSQL ReturnStmt AST node and store the SQL AST in it.
          */
          Driver sql_driver;
          sql_driver.parse($2, false);

          std::shared_ptr<lingodb::ast::AstNode> query_ast = nullptr;
          if (!sql_driver.result.empty()) {
              // A SQL string can contain multiple statements. We'll take the first.
              query_ast = sql_driver.result[0];
          }

          $$ = std::make_shared<lingodb::ast::pl::ReturnStmt>(query_ast);
      }
      ;
%%
void
lingodb::frontend::pl::parser::error (const location_type& l, const std::string& m)
{
      std::ostringstream oss;
      oss << m << " at "
          << l
          ;
      throw std::runtime_error(oss.str());

}

