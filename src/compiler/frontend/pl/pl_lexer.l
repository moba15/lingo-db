%{
#include <string>
#include "lingodb/compiler/frontend/pl/pl_driver.h"
#include "lingodb/compiler/frontend/pl/pl_parser.hpp"

// Buffer to accumulate the SQL text
static std::string sql_buffer;
// Counter for parenthesis nesting
static int paren_level;
%}

%option noyywrap nounput noinput batch
%option prefix="plpgsql_"

/*
 * This state is entered when the 'RETURN' keyword is found.
 * It captures the subsequent SQL expression as a single string.
*/
%x SQL_TEXT_CAPTURE

%%
%{
    // Convenience reference to the driver's location tracking
    lingodb::location& loc = drv.location;
    loc.step();
%}

[ 	
]+          loc.lines (yyleng); /* Ignore whitespace */
"DECLARE"           return lingodb::frontend::pl::parser::make_DECLARE(loc);
"BEGIN"             return lingodb::frontend::pl::parser::make_BEGIN(loc);
"END"               return lingodb::frontend::pl::parser::make_END(loc);
";"                 return lingodb::frontend::pl::parser::make_SEMICOLON(loc);
<<EOF>>             return lingodb::frontend::pl::parser::make_YYEOF(loc);

/*
 * When RETURN is encountered, we switch to the SQL_TEXT_CAPTURE state.
 * The lexer will then use the rules for that state to capture the SQL.
*/
"RETURN"            {
    BEGIN(SQL_TEXT_CAPTURE);
    paren_level = 0;
    sql_buffer.clear();
    return lingodb::frontend::pl::parser::make_RETURN_P(loc);
}

<SQL_TEXT_CAPTURE>{
    /*
     * This is the core logic for capturing the SQL expression.
     * It counts parentheses to find the end of the expression, which is
     * marked by a semicolon at parenthesis level 0.
     *
     * NOTE: This is a simplified implementation. It does not correctly
     * handle parentheses inside of string literals or comments. A more
     * robust solution would require states for strings and comments,
     * similar to the main SQL lexer.
    */
    "("         { sql_buffer.append(yytext); paren_level++; }
    ")"         { sql_buffer.append(yytext); if (paren_level > 0) paren_level--; }
    ";"         {
                    if (paren_level == 0) {
                        yyless(0); // Put the semicolon back for the main parser
                        BEGIN(INITIAL);
                        return lingodb::frontend::pl::parser::make_SQL_TEXT(sql_buffer, loc);
                    } else {
                        sql_buffer.append(yytext);
                    }
                }
    /* Capture any other character, one by one. */
    .|
        { sql_buffer.append(yytext); }
}

.                   { /* For now, we ignore any other characters in PL/SQL */ }

%%

/*
 * These functions are called by the driver to control the lexer.
 * They allow parsing from a string.
*/
static YY_BUFFER_STATE pl_buffer_state = nullptr;

void pl_scan_string(const char* s) {
    pl_buffer_state = plpgsql__scan_string(s);
    plpgsql__switch_to_buffer(pl_buffer_state);
}

void pl_delete_buffer() {
    if (pl_buffer_state) {
        plpgsql__delete_buffer(pl_buffer_state);
        pl_buffer_state = nullptr;
    }
}

int plpgsql_wrap() {
    return 1;
}