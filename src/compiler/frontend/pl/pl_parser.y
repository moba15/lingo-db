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
%token <int> ICONST
%token <uint64_t>	    INTEGER_VALUE	"integer_value"
%token <std::string>	FCONST
%token <std::string>     IDENT
%token <std::string>	IDENTIFIER	"identifier"
%token <std::string>	STRING_VALUE
%token <std::string>	BIT_VALUE	"bit_string"
%token <std::string>	HEX_VALUE	"hex_string"
%token <std::string>	NATIONAL_VALUE	"nat_string"
%token 			LP		"("
%token 			RP		")"
%token 			LB		"["
%token 			RB		"]"
%token 			DOT		"."
%token          PERCENT "%"
%token 			COMMA		","
%token 			SEMICOLON	";"
%token 			PLUS		"+"
%token 			SLASH		"/"
%token 			STAR		"*"
%token 			MINUS		"-"
%token 			EQUAL		"="
%token 			NOT_EQUAL	"<>"
%token 			LESS_EQUAL	"<="
%token 			LESS		"<"
%token 			GREATER_EQUAL	">="
%token 			GREATER		">"
%token      HAT         "^"
%token 			QUOTE		"'"

%token <std::string>  PERCENT EQUAL NOT_EQUAL LESS_EQUAL LESS GREATER_EQUAL GREATER QUOTE MINUS STAR PLUS SLASH Op
                      DECLARE BEGIN END       RETURN_P
/*
 * Taken directly from postgres grammatic
 * TODO LINK
**/
%token <std::string> TEST

/*
 * The grammar thinks these are keywords, but they are not in the kwlist.h
 * list and so can never be entered directly.  The filter in parser.c
 * creates these tokens when required (based on looking one token ahead).
 *
 * NOT_LA exists so that productions such as NOT LIKE can be given the same
 * precedence as LIKE; otherwise they'd effectively have the same precedence
 * as NOT, at least with respect to their left-hand subexpression.
 * FORMAT_LA, NULLS_LA, WITH_LA, and WITHOUT_LA are needed to make the grammar
 * LALR(1).
 */




%printer {  } <*>;

%%
%start pl_function;


pl_function:
  pl_block
;



pl_block:
  decl_sect BEGIN proc_sect END SEMICOLON
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
  proc_sect proc_stmt
  {

  }
  | %empty
;
//TODO
proc_stmt:
  pl_block SEMICOLON
  | stmt_return
;

    stmt_return:
      RETURN_P
      {
      std::ostringstream buf;
            int paren_depth = 0;
            for (;;) {






          }
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

