%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "lingodb/compiler/plpgsql-frontend/pl_driver.h"
# include "parser.hpp"
# include <algorithm>
#include <lingodb/compiler/plpgsql-frontend/sql-parser/gen/location.hh>

// State for dollar-quoted strings
static std::string literal_buffer;
static std::string dolq_tag;

%}

%{
#if defined __clang__
# define CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#endif

// Clang and ICC like to pretend they are GCC.
#if defined __GNUC__ && !defined __clang__ && !defined __ICC
# define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#endif

// Pacify warnings in yy_init_buffer (observed with Flex 2.6.4)
// and GCC 6.4.0, 7.3.0 with -O3.
#if defined GCC_VERSION && 600 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wnull-dereference"
#endif

// This example uses Flex's C back end, yet compiles it as C++.
// So expect warnings about C style casts and NULL.
#if defined CLANG_VERSION && 500 <= CLANG_VERSION
# pragma clang diagnostic ignored "-Wold-style-cast"
# pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#elif defined GCC_VERSION && 407 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wold-style-cast"
# pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)

// Old versions of Flex (2.5.35) generate an incomplete documentation comment.
//
//  In file included from src/scan-code-c.c:3:
//  src/scan-code.c:2198:21: error: empty paragraph passed to '@param' command
//        [-Werror,-Wdocumentation]
//   * @param line_number
//     ~~~~~~~~~~~~~~~~~^
//  1 error generated.
#if FLEX_VERSION < 206 && defined CLANG_VERSION
# pragma clang diagnostic ignored "-Wdocumentation"
#endif

// Old versions of Flex (2.5.35) use 'register'.  Warnings introduced in
// GCC 7 and Clang 6.
#if FLEX_VERSION < 206
# if defined CLANG_VERSION && 600 <= CLANG_VERSION
#  pragma clang diagnostic ignored "-Wdeprecated-register"
# elif defined GCC_VERSION && 700 <= GCC_VERSION
#  pragma GCC diagnostic ignored "-Wregister"
# endif
#endif

#if FLEX_VERSION < 206
# if defined CLANG_VERSION
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wdocumentation"
#  pragma clang diagnostic ignored "-Wshorten-64-to-32"
#  pragma clang diagnostic ignored "-Wsign-conversion"
# elif defined GCC_VERSION
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
# endif
#endif

// Flex 2.6.4, GCC 9
// warning: useless cast to type 'int' [-Wuseless-cast]
// 1361 |   YY_CURRENT_BUFFER_LVALUE->yy_buf_size = (int) (new_size - 2);
//      |                                                 ^
#if defined GCC_VERSION && 900 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

#define MAKE_KEYWORD(token_name) \
    return lingodb::frontend::pl::parser::make_##token_name(std::string(yytext),loc);


%}

%option noyywrap nounput noinput batch debug prefix="plpgsql_"

%{
  // A number symbol corresponding to the value in S.


%}
%x xdolq

space				[ \t\f\v]+
newline				[\n\r]
non_newline			[^\n\r]

comment			("--"{non_newline}*)

whitespace		({comment}|{space})+

decdigit        [0-9]
decinteger		{decdigit}+

numeric         ({decinteger}\.{decinteger}?)|(\.{decinteger})
op_chars		    [\~\!\@\#\^\&\|\`\?\+\-\*\/\%\<\>\=]
ident_start     [a-zA-Z]
ident_cont      [a-zA-Z0-9_%]
identifier      {ident_start}{ident_cont}*
quoted_identifier   \"([^\"]|\"\")*\"
squote			\'
dquote			\"
strChars ([^'])
str       {squote}({strChars}|{op_chars}|[ ])*{squote}

all ([^'])

dolq_start		[A-Za-z\200-\377_]
dolq_cont		[A-Za-z\200-\377_0-9]
dolqdelim		\$({dolq_start}{dolq_cont}*)?\$
dolqfailed		\${dolq_start}{dolq_cont}*
dolqinside		[^$]+

operator		{op_chars}+


boolean [fF][aA][lL][sS][eE]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%option caseless
%%
%{
  // A handy shortcut to the location held by the driver.
  lingodb::frontend::pl::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}
"("             { return lingodb::frontend::pl::parser::make_LP(loc); }
")"             { return lingodb::frontend::pl::parser::make_RP(loc); }
"["             { return lingodb::frontend::pl::parser::make_LB(loc); }
"]"             { return lingodb::frontend::pl::parser::make_RB(loc); }
"."             { return lingodb::frontend::pl::parser::make_DOT(loc); }
","             { return lingodb::frontend::pl::parser::make_COMMA(loc); }
";"             { return lingodb::frontend::pl::parser::make_SEMICOLON(loc); }
"+"             { MAKE_KEYWORD(PLUS); }
"-"             { MAKE_KEYWORD(MINUS); }
"/"             { MAKE_KEYWORD(SLASH); }
"*"             { MAKE_KEYWORD(STAR); }
"="             { MAKE_KEYWORD(EQUAL); }
"%"             { MAKE_KEYWORD(PERCENT); }
"<>"            { MAKE_KEYWORD(NOT_EQUAL); }
"!="            { MAKE_KEYWORD(NOT_EQUAL); }
"<="            { MAKE_KEYWORD(LESS_EQUAL); }
"<"             { MAKE_KEYWORD(LESS); }
">="            { MAKE_KEYWORD(GREATER_EQUAL); }
">"             { MAKE_KEYWORD(GREATER); }
{newline}              { loc.lines();}
BEGIN           {MAKE_KEYWORD(BEGIN); }

DECLARE         { MAKE_KEYWORD(DECLARE); }

END             { MAKE_KEYWORD(END); }

RETURN          { MAKE_KEYWORD(RETURN_P); }

<<EOF>>    return lingodb::frontend::pl::parser::make_YYEOF (loc);

{whitespace}    {
                  /*Ignore*/
                }
{identifier}    {
                    std::string ident(yytext);
                    std::transform(ident.begin(), ident.end(), ident.begin(),
                        [](unsigned char c){ return std::tolower(c); });
                    return lingodb::frontend::pl::parser::make_IDENTIFIER(std::move(ident), loc);
                }
{quoted_identifier} {
                      std::string text(yytext);
                      std::string trimmed(text.begin() + 1, text.end()-1);
                      return lingodb::frontend::pl::parser::make_IDENTIFIER(std::move(trimmed), loc);

                    }
{decinteger}	{
				    return lingodb::frontend::pl::parser::make_ICONST(std::stoul(yytext), loc);
			    }
{numeric}	{
				    return lingodb::frontend::pl::parser::make_FCONST(yytext, loc);
			    }
{str} {
  std::string ident(yytext);
  ident.erase(0,1);
  ident.erase(ident.size()-1,1);
  return lingodb::frontend::pl::parser::make_STRING_VALUE(std::move(ident), loc);
}

{all} {
  std::string ident(yytext);
  ident.erase(0,1);
  ident.erase(ident.size()-1,1);
  return lingodb::frontend::pl::parser::make_ALL(std::move(ident), loc);
}
{operator} {
  std::string ident(yytext);

  /*
   * For SQL compatibility multi char operators cannot end with '+' and '-'.
   * They should be parsed as two tokens
  */
  //Remove '+' and '-' at the end if necessary
  while(ident[ident.length() - 1] == '-' || ident[ident.length() - 1] == '+') {
    ident.erase(ident.length() - 1);
  }
  yyless(ident.length());

  if(ident == "<=") {
    return lingodb::frontend::pl::parser::make_LESS_EQUAL("<=", loc);
  } else if(ident == "<>" || ident == "!=") {
    return lingodb::frontend::pl::parser::make_NOT_EQUAL("<>", loc);
  } else if(ident == ">=") {
    return lingodb::frontend::pl::parser::make_GREATER_EQUAL(">=", loc);
  }

  return lingodb::frontend::pl::parser::make_Op(std::move(ident), loc);
}


<xdolq>{dolqinside} {
    literal_buffer.append(yytext, yyleng);
    // Handle newlines inside the string for correct location tracking
    for (int i = 0; i < yyleng; ++i) {
        if (yytext[i] == '\\n' || yytext[i] == '\\r') {
            loc.lines();
        }
    }
    loc.columns(yyleng);
}

<xdolq>"$" {
    literal_buffer.append("$");
    loc.columns(1);
}

<xdolq>{dolqdelim} {
    if (dolq_tag == yytext) {
        BEGIN(INITIAL);
        loc.columns(yyleng);
        return lingodb::frontend::pl::parser::make_STRING_VALUE(std::move(literal_buffer), loc);
    } else {
        literal_buffer.append(yytext, yyleng);
        loc.columns(yyleng);
    }
}

<xdolq><<EOF>> {
    return lingodb::frontend::pl::parser::make_YYEOF(loc);
}

{dolqdelim} {
    dolq_tag = std::string(yytext);
    literal_buffer.clear();
    BEGIN(xdolq);
    loc.step();
}

%%



void
pl_driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  if(file.starts_with(":")) {
     const char* sql = file.c_str() + 1; // Skip the ":" prefix
     YY_BUFFER_STATE buffer = yy_scan_string(sql);
     yy_switch_to_buffer(buffer);
  }
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void
pl_driver::scan_end ()
{
  if (YY_CURRENT_BUFFER) {
      yy_delete_buffer(YY_CURRENT_BUFFER);
   }
   if (yyin && yyin != stdin) {
      fclose (yyin);
   }
}