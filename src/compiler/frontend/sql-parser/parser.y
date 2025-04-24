%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.2"
%header

%define api.token.raw
%define api.namespace    { lingodb }
%define api.token.constructor
%define api.value.type variant
%define parse.assert



%code requires {
  # include <string>
  #include <iostream>
  #include <memory>
  #include <vector>

  #include "lingodb/compiler/frontend/sql-parser/node_factory.h"
  #include "lingodb/compiler/frontend/sql-parser/query_node/list.h"
  #include "lingodb/compiler/frontend/sql-parser/tableref.h"
  #include "lingodb/compiler/frontend/sql-parser/parsed_expression/list.h"
  #include "lingodb/compiler/frontend/sql-parser/tableref/list.h"
  #include "lingodb/compiler/frontend/sql-parser/common/binding_alias.h"
  class driver;
}

// The parsing context.
%param { driver& drv }



%define parse.trace
%define parse.error detailed
%define parse.lac full
%locations
%code {
  #include <iostream>
  #include <memory>
  #include "lingodb/compiler/frontend/sql-parser/driver.h"
  #include "lingodb/compiler/frontend/sql-parser/node_factory.h"

  #define mkNode drv.nf.node
  #define mkListShared drv.nf.listShared
  #define mkList drv.nf.list
  #define mkNotNode drv.nf.notNode


}

%define api.token.prefix {TOK_}
//TODO check if int?
%token <int> ICONST
%token <uint64_t>	INTEGER_VALUE	"integer_value"
%token <double>		FLOAT_VALUE	"float_value"
%token <std::string>	IDENTIFIER	"identifier"
%token <std::string>	STRING_VALUE	"string"
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
%token          HAT         "^"
%token 			QUOTE		"'"
%token 			CONCAT		"||"
/* 
 * Taken directly from postgres grammatic 
 * TODO LINK
**/
%token ABORT_P ABSENT ABSOLUTE_P ACCESS ACTION ADD_P ADMIN AFTER
	AGGREGATE ALL ALSO ALTER ALWAYS ANALYSE ANALYZE AND ANY ARRAY AS ASC
	ASENSITIVE ASSERTION ASSIGNMENT ASYMMETRIC ATOMIC AT ATTACH ATTRIBUTE AUTHORIZATION

	BACKWARD BEFORE BEGIN_P BETWEEN BIGINT BINARY BIT
	BOOLEAN_P BOTH BREADTH BY

	CACHE CALL CALLED CASCADE CASCADED CASE CAST CATALOG_P CHAIN CHAR_P
	CHARACTER CHARACTERISTICS CHECK CHECKPOINT CLASS CLOSE
	CLUSTER COALESCE COLLATE COLLATION COLUMN COLUMNS COMMENT COMMENTS COMMIT
	COMMITTED COMPRESSION CONCURRENTLY CONDITIONAL CONFIGURATION CONFLICT
	CONNECTION CONSTRAINT CONSTRAINTS CONTENT_P CONTINUE_P CONVERSION_P COPY
	COST CREATE CROSS CSV CUBE CURRENT_P
	CURRENT_CATALOG CURRENT_DATE CURRENT_ROLE CURRENT_SCHEMA
	CURRENT_TIME CURRENT_TIMESTAMP CURRENT_USER CURSOR CYCLE

	DATA_P DATABASE DAY_P DEALLOCATE DEC DECIMAL_P DECLARE DEFAULT DEFAULTS
	DEFERRABLE DEFERRED DEFINER DELETE_P DELIMITER DELIMITERS DEPENDS DEPTH DESC
	DETACH DICTIONARY DISABLE_P DISCARD DISTINCT DO DOCUMENT_P DOMAIN_P
	DOUBLE_P DROP

	EACH ELSE EMPTY_P ENABLE_P ENCODING ENCRYPTED END_P ENFORCED ENUM_P ERROR_P
	ESCAPE EVENT EXCEPT EXCLUDE EXCLUDING EXCLUSIVE EXECUTE EXISTS EXPLAIN
	EXPRESSION EXTENSION EXTERNAL EXTRACT

	FALSE_P FAMILY FETCH FILTER FINALIZE FIRST_P FLOAT_P FOLLOWING FOR
	FORCE FOREIGN FORMAT FORWARD FREEZE FROM FULL FUNCTION FUNCTIONS

	GENERATED GLOBAL GRANT GRANTED GREATEST GROUP_P GROUPING GROUPS

	HANDLER HAVING HEADER_P HOLD HOUR_P

	IDENTITY_P IF_P ILIKE IMMEDIATE IMMUTABLE IMPLICIT_P IMPORT_P IN_P INCLUDE
	INCLUDING INCREMENT INDENT INDEX INDEXES INHERIT INHERITS INITIALLY INLINE_P
	INNER_P INOUT INPUT_P INSENSITIVE INSERT INSTEAD INT_P INTEGER
	INTERSECT INTERVAL INTO INVOKER IS ISNULL ISOLATION

	JOIN JSON JSON_ARRAY JSON_ARRAYAGG JSON_EXISTS JSON_OBJECT JSON_OBJECTAGG
	JSON_QUERY JSON_SCALAR JSON_SERIALIZE JSON_TABLE JSON_VALUE

	KEEP KEY KEYS

	LABEL LANGUAGE LARGE_P LAST_P LATERAL_P
	LEADING LEAKPROOF LEAST LEFT LEVEL LIKE LIMIT LISTEN LOAD LOCAL
	LOCALTIME LOCALTIMESTAMP LOCATION LOCK_P LOCKED LOGGED

	MAPPING MATCH MATCHED MATERIALIZED MAXVALUE MERGE MERGE_ACTION METHOD
	MINUTE_P MINVALUE MODE MONTH_P MOVE

	NAME_P NAMES NATIONAL NATURAL NCHAR NESTED NEW NEXT NFC NFD NFKC NFKD NO
	NONE NORMALIZE NORMALIZED
	NOT NOTHING NOTIFY NOTNULL NOWAIT NULL_P NULLIF
	NULLS_P NUMERIC

	OBJECT_P OBJECTS_P OF OFF OFFSET OIDS OLD OMIT ON ONLY OPERATOR OPTION OPTIONS OR
	ORDER ORDINALITY OTHERS OUT_P OUTER_P
	OVER OVERLAPS OVERLAY OVERRIDING OWNED OWNER

	PARALLEL PARAMETER PARSER PARTIAL PARTITION PASSING PASSWORD PATH
	PERIOD PLACING PLAN PLANS POLICY
	POSITION PRECEDING PRECISION PRESERVE PREPARE PREPARED PRIMARY
	PRIOR PRIVILEGES PROCEDURAL PROCEDURE PROCEDURES PROGRAM PUBLICATION

	//!QUOTE 
    QUOTES

	RANGE READ REAL REASSIGN RECURSIVE REF_P REFERENCES REFERENCING
	REFRESH REINDEX RELATIVE_P RELEASE RENAME REPEATABLE REPLACE REPLICA
	RESET RESTART RESTRICT RETURN RETURNING RETURNS REVOKE RIGHT ROLE ROLLBACK ROLLUP
	ROUTINE ROUTINES ROW ROWS RULE

	SAVEPOINT SCALAR SCHEMA SCHEMAS SCROLL SEARCH SECOND_P SECURITY SELECT
	SEQUENCE SEQUENCES
	SERIALIZABLE SERVER SESSION SESSION_USER SET SETS SETOF SHARE SHOW
	SIMILAR SIMPLE SKIP SMALLINT SNAPSHOT SOME SOURCE SQL_P STABLE STANDALONE_P
	START STATEMENT STATISTICS STDIN STDOUT STORAGE STORED STRICT_P STRING_P STRIP_P
	SUBSCRIPTION SUBSTRING SUPPORT SYMMETRIC SYSID SYSTEM_P SYSTEM_USER

	TABLE TABLES TABLESAMPLE TABLESPACE TARGET TEMP TEMPLATE TEMPORARY TEXT_P THEN
	TIES TIME TIMESTAMP TO TRAILING TRANSACTION TRANSFORM
	TREAT TRIGGER TRIM TRUE_P
	TRUNCATE TRUSTED TYPE_P TYPES_P

	UESCAPE UNBOUNDED UNCONDITIONAL UNCOMMITTED UNENCRYPTED UNION UNIQUE UNKNOWN
	UNLISTEN UNLOGGED UNTIL UPDATE USER USING

	VACUUM VALID VALIDATE VALIDATOR VALUE_P VALUES VARCHAR VARIADIC VARYING
	VERBOSE VERSION_P VIEW VIEWS VIRTUAL VOLATILE

	WHEN WHERE WHITESPACE_P WINDOW WITH WITHIN WITHOUT WORK WRAPPER WRITE

	XML_P XMLATTRIBUTES XMLCONCAT XMLELEMENT XMLEXISTS XMLFOREST XMLNAMESPACES
	XMLPARSE XMLPI XMLROOT XMLSERIALIZE XMLTABLE

	YEAR_P YES_P

	ZONE


%type <std::shared_ptr<lingodb::ast::QueryNode>> stmtmulti select_no_parens select_clause toplevel_stmt stmt simple_select SelectStmt select_with_parens

%type<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>> opt_target_list target_list

%type<std::shared_ptr<lingodb::ast::ParsedExpression>> target_el a_expr c_expr where_clause

%type<lingodb::ast::    jointCondOrUsingCols> join_qual

%type<std::shared_ptr<lingodb::ast::ColumnRefExpression>> columnref

%type<std::shared_ptr<lingodb::ast::TableRef>> from_clause table_ref from_list joined_table

%type<std::string>  ColId ColLabel indirection attr_name indirection_el 
                    qualified_name relation_expr alias_clause opt_alias_clause 
                    name

%type<std::vector<std::string>> name_list

%type<std::shared_ptr<lingodb::ast::ConstantExpression>> Iconst AexprConst 

/*%type <nodes::RelExpression>		simple_select
%type <std::shared_ptr<nodes::Query>> select_no_parens
%type <std::shared_ptr<nodes::SelectStatement>> SelectStatement
%type <std::vector<nodes::RelExpression>>	from_clause
%type <std::vector<nodes::RelExpression>>	from_list
%type <nodes::RelExpression>						table_ref
%type <std::shared_ptr<nodes::QualifiedName>>					qualified_name
%type <std::vector<std::string>>					qualifier_list
%type <std::shared_ptr<nodes::RowExpr>>					target_list
%type <std::shared_ptr<nodes::QualifiedName>>					target_element*/
//%token <int> NUMBER "number"
//%nterm <int> exp



/* Precedence: lowest to highest */
%left		UNION EXCEPT
%left		INTERSECT
%left		OR
%left		AND
%right		NOT
%nonassoc	IS ISNULL NOTNULL	/* IS sets precedence for IS NULL, etc */
%nonassoc	GREATER LESS EQUAL LESS_EQUALS GREATER_EQUALS NOT_EQUALS
%nonassoc	BETWEEN IN_P LIKE ILIKE SIMILAR NOT_LA
%nonassoc	ESCAPE			/* ESCAPE must be just above LIKE/ILIKE/SIMILAR */


%nonassoc	UNBOUNDED NESTED /* ideally would have same precedence as IDENT */
%nonassoc	IDENT PARTITION RANGE ROWS GROUPS PRECEDING FOLLOWING CUBE ROLLUP
			SET KEYS OBJECT_P SCALAR VALUE_P WITH WITHOUT PATH
%left		Op OPERATOR		/* multi-character ops and user-defined operators */
%left		PLUS MINUS
%left		STAR SLASH PERCENT
%left		HAT
/* Unary Operators */
%left		AT				/* sets precedence for AT TIME ZONE, AT LOCAL */
%left		COLLATE
%right		UMINUS
%left		LB RB
%left		LP RP
%left		TYPECAST
%left		DOT
/*
 * These might seem to be low-precedence, but actually they are not part
 * of the arithmetic hierarchy at all in their use as JOIN operators.
 * We make them high-precedence to support their use as function names.
 * They wouldn't be given a precedence at all, were it not that we need
 * left-associativity among the JOIN rules themselves.
 */
%left		JOIN CROSS LEFT FULL RIGHT INNER_P NATURAL


%printer { std::cout << ""; } <*>;

%%
%start parse_toplevel;

/*
 * We parse a list of statements, but if there are any special modes first we can add them here
 */
parse_toplevel: 
    stmtmulti {drv.result = $stmtmulti;}
    ;
/*
 * Allows 
 */
 //TODO Allow multiple
stmtmulti: 
    stmtmulti SEMICOLON toplevel_stmt {}
    | toplevel_stmt {$$=$1;}
    ;

toplevel_stmt:
    stmt {$$=$1;}
  //TODO Add Later  | TransactionStmtLegacy 
  ;
/*
 * TODO Add the different Statement Types, like Create, Copy etc
*/
stmt: 
 SelectStmt {$$=$1;}
 ;

 SelectStmt: 
    select_no_parens {$$=$select_no_parens;}
    | select_with_parens {$$=$select_with_parens;}
    ;
select_with_parens:
    LP select_no_parens RP {$$=$select_no_parens;}
    | LP select_with_parens RP {$$=$2;}
    ;

select_no_parens: 
    simple_select {$$=$1;}
    //TODO | select_clause sort_clause
    //TODO | select_clause opt_sort_clause for_locking_clause opt_select_limit 
    //TODO | select_clause opt_sort_clause select_limit opt_for_locking_clause
    //TODO | with_clause select_clause
    //TODO | with_clause select_clause sort_clause
    //TODO | with_clause select_clause opt_sort_clause for_locking_clause opt_select_limit
    //TODO | with_clause select_clause opt_sort_clause select_limit opt_for_locking_clause
    ;
select_clause: 
    simple_select {$$ = $1;}
    | select_with_parens 
    ;

/*
 * This rule parses SELECT statements that can appear within set operations,
 * including UNION, INTERSECT and EXCEPT.  '(' and ')' can be used to specify
 * the ordering of the set operations.	Without '(' and ')' we want the
 * operations to be ordered per the precedence specs at the head of this file.
 *
 * As with select_no_parens, simple_select cannot have outer parentheses,
 * but can have parenthesized subclauses.
 *
 * It might appear that we could fold the first two alternatives into one
 * by using opt_distinct_clause.  However, that causes a shift/reduce conflict
 * against INSERT ... SELECT ... ON CONFLICT.  We avoid the ambiguity by
 * requiring SELECT DISTINCT [ON] to be followed by a non-empty target_list.
 *
 * Note that sort clauses cannot be included at this level --- SQL requires
 *		SELECT foo UNION SELECT bar ORDER BY baz
 * to be parsed as
 *		(SELECT foo UNION SELECT bar) ORDER BY baz
 * not
 *		SELECT foo UNION (SELECT bar ORDER BY baz)
 * !Likewise for WITH, FOR UPDATE and LIMIT.  Therefore, those clauses are
 * described as part of the select_no_parens production, not simple_select.
 * This does not limit functionality, because you can reintroduce these
 * clauses inside parentheses.
 *
 *NOTE: only the leftmost component SelectStmt should have INTO.
 * !However, this is not checked by the grammar; parse analysis must check it.
 */
 //! Check for what the different options are!
simple_select: 
    SELECT opt_all_clause opt_target_list 
    //TODO into_clause 
    from_clause where_clause
    //TODO group_clause having_clause window_clause
    {
        auto t = mkNode<lingodb::ast::SelectNode>(@$);
        t->select_list = $opt_target_list;
        t->where_clause = $where_clause;
        t->from_clause = $from_clause;
        $$ = t;
    }
    //TODO | SELECT distinct_clause target_list into_clause from_clause where_clause group_clause having_clause window_clause
    //TODO | values_clause
    //TODO | TABLE relation_expr
    //TODO | select_clause UNION set_quantifier select_clause
    //TODO | select_clause INTERSECT set_quantifier select_clause
    //TODO | select_clause EXCEPT set_quantifier select_clause
    ;


/*****************************************************************************
 *
 *	clauses common to all Optimizable Stmts:
 *		from_clause		- allow list of both JOIN expressions and table names
 *		where_clause	- qualifications for joins or restrictions
 *
 *****************************************************************************/
 from_clause:
			FROM from_list							{ $$=$from_list; }
			| %empty								{  }
            ;

from_list: 
    table_ref { $$=$1;}
    | from_list COMMA table_ref
    ;


/*
 * table_ref is where an alias clause can be attached.
 */
 //TODO add missing rules
table_ref: 
    relation_expr opt_alias_clause 
    { 
        //TODO Alias clause
        //TODO schema 
        //TODO for now it is very simplyfied
        lingodb::ast::TableDescription desc{"", "", $relation_expr };
        auto tableref = mkNode<lingodb::ast::BaseTableRef>(@$, desc);
        tableref->alias = $opt_alias_clause;
        $$ = tableref;

    }
    | joined_table { $$ = $1;}
    | LP joined_table RP alias_clause
    | joined_table opt_alias_clause


    ;
/*
 * It may seem silly to separate joined_table from table_ref, but there is
 * method in SQL's madness: if you don't do it this way you get reduce-
 * reduce conflicts, because it's not clear to the parser generator whether
 * to expect alias_clause after ')' or not.  For the same reason we must
 * treat 'JOIN' and 'join_type JOIN' separately, rather than allowing
 * join_type to expand to empty; if we try it, the parser generator can't
 * figure out when to reduce an empty join_type right after table_ref.
 *
 * Note that a CROSS JOIN is the same as an unqualified
 * INNER JOIN, and an INNER JOIN/ON has the same shape
 * but a qualification expression to limit membership.
 * A NATURAL JOIN implicitly matches column names between
 * tables and the shape is determined by which columns are
 * in common. We'll collect columns during the later transformations.
 */
joined_table: 
    LP joined_table RP {$$=$2;}
    | table_ref CROSS JOIN table_ref
    | table_ref join_type JOIN table_ref join_qual
    | table_ref[left] JOIN table_ref[right] join_qual 
    {
        //TODO find out correct JoinCondType
        auto join = mkNode<lingodb::ast::JoinRef>(@$, lingodb::ast::JoinType::INNER, lingodb::ast::JoinCondType::REGULAR);
        join->left = $left;
        join->right = $right;
        join->condition = $join_qual;
        $$ = join;
    }
    | table_ref NATURAL join_type JOIN table_ref
    | table_ref NATURAL JOIN table_ref
    ;

alias_clause: 
    AS ColId LP name_list RP
    | AS ColId {$$ = $ColId;}
    | ColId LP name_list RP
    | ColId {$$ = $ColId;} //TODO Check if correct
    ;



opt_alias_clause: 
    alias_clause {$$ = $alias_clause;}
    | %empty
    ;

opt_alias_clause_for_join_using:
    AS ColId
    | %empty
    ;


join_type: 
    FULL opt_outer
    | LEFT opt_outer
    | RIGHT opt_outer 
    | INNER_P
    ;

opt_outer: 
    OUTER_P
    | %empty
    ;

/* JOIN qualification clauses
 * Possibilities are:
 *	USING ( column list ) [ AS alias ]
 *						  allows only unqualified column names,
 *						  which must match between tables.
 *	ON expr allows more general qualifications.
 *
 * We return USING as a two-element List (the first item being a sub-List
 * of the common column names, and the second either an Alias item or NULL).
 * An ON-expr will not be a List, so it can be told apart that way.
 */

join_qual:
    USING LP name_list RP //TODO not allowing alias after USING for now opt_alias_clause_for_join_using 
    {
        auto name_list = $name_list;
        auto list = mkListShared<lingodb::ast::ColumnRefExpression>();
        for(auto& name : name_list) {
            list.emplace_back(mkNode<lingodb::ast::ColumnRefExpression>(@$,name));
        }
        $$ = list;

    }
    | ON a_expr {$$=$a_expr;}
    ;

relation_expr:
    qualified_name {$$ = $qualified_name;}
    | extended_relation_expr
    ;
    
extended_relation_expr: 
    qualified_name STAR
    | ONLY qualified_name
    | ONLY LP qualified_name RP
    ;

opt_all_clause:
    ALL
    | %empty
    ;

/*****************************************************************************
 *
 *	expression grammar
 *
 *****************************************************************************/
 //TODO Add missing expressions, for instance func_expr
/* 
 * POSGRES
 * General expressions
 * This is the heart of the expression syntax.
 *
 * We have two expression types: a_expr is the unrestricted kind, and
 * b_expr is a subset that must be used in some places to avoid shift/reduce
 * conflicts.  For example, we can't do BETWEEN as "BETWEEN a_expr AND a_expr"
 * because that use of AND conflicts with AND as a boolean operator.  So,
 * b_expr is used in BETWEEN and we remove boolean keywords from b_expr.
 *
 * Note that '(' a_expr ')' is a b_expr, so an unrestricted expression can
 * always be used by surrounding it with parens.
 *
 * c_expr is all the productions that are common to a_expr and b_expr;
 * it's factored out just to eliminate redundant coding.
 *
 * Be careful of productions involving more than one terminal token.
 * By default, bison will assign such productions the precedence of their
 * last terminal, but in nearly all cases you want it to be the precedence
 * of the first terminal instead; otherwise you will not get the behavior
 * you expect!  So we use %prec annotations freely to set precedences.
 */

where_clause: 
    WHERE a_expr {$$=$a_expr;}
    | %empty 
    ;
/*
TODO
 * Add missing rules
*/
a_expr: 
    c_expr { $$ = $c_expr;}
   //TODO | a_expr TYPECAST Typename
    //TODO | a-expr COLLATE any_name
    //TODO | a_expr AT TIME ZONE a_expr
    //TODO | a_expr AT LOCAL
    | PLUS a_expr
    | MINUS a_expr
    | a_expr PLUS a_expr
    | a_expr MINUS a_expr
    | a_expr STAR a_expr
    | a_expr SLASH a_expr
    | a_expr PERCENT a_expr
    | a_expr HAT a_expr
    | a_expr LESS a_expr 
    {
        $$ = mkNode<lingodb::ast::ComparisonExpression>(@$, lingodb::ast::ExpressionType::COMPARE_LESSTHAN, $1, $3 );
    }
    | a_expr GREATER a_expr
    {
        $$ = mkNode<lingodb::ast::ComparisonExpression>(@$, lingodb::ast::ExpressionType::COMPARE_GREATERTHAN, $1, $3 );
    }
    | a_expr EQUAL a_expr
    {
        $$ = mkNode<lingodb::ast::ComparisonExpression>(@$, lingodb::ast::ExpressionType::COMPARE_EQUAL, $1, $3 );
    }
    | a_expr LESS_EQUAL a_expr
    {
        $$ = mkNode<lingodb::ast::ComparisonExpression>(@$, lingodb::ast::ExpressionType::COMPARE_LESSTHANOREQUALTO, $1, $3 );
    }
    | a_expr GREATER_EQUAL a_expr
    {
        $$ = mkNode<lingodb::ast::ComparisonExpression>(@$, lingodb::ast::ExpressionType::COMPARE_GREATERTHANOREQUALTO, $1, $3 );
    }
    | a_expr NOT_EQUAL a_expr
    {
        $$ = mkNode<lingodb::ast::ComparisonExpression>(@$, lingodb::ast::ExpressionType::COMPARE_NOTEQUAL, $1, $3 );
    }
    | a_expr AND a_expr 
    {
       $$ = mkNode<lingodb::ast::ConjunctionExpression>(@$, lingodb::ast::ExpressionType::CONJUNCTION_AND , $1, $3);
    }
    | a_expr OR a_expr
    {
        $$ = mkNode<lingodb::ast::ConjunctionExpression>(@$, lingodb::ast::ExpressionType::CONJUNCTION_OR, $1, $3);
    }
    | NOT a_expr
    ;
/*
 * Productions that can be used in both a_expr and b_expr.
 * Note: productions that refer recursively to a_expr or b_expr mostly cannot appear here.
 */
c_expr: 
    columnref {$$ = $columnref;}
    | AexprConst {$$=$1;}
    //TODO | PARAM opt_indirection
    //TODO
    | LP a_expr RP {$$=$2;}//opt_indirection
    //TODO | case_expr
    //TODO | func_expr
    //TODO | select_with_parens
    //TODO | select_with_parens indirection 
    //TODO | EXISTS select_with_parens
    //TODO | ARRAY select_with_parens
    //TODO | ARRAY array_expr
    //TODO | explicit_row
    //TODO | implicit_row
    //TODO | GROUPING LP expr_list RP
    ;


columnref: 
    ColId {$$ = mkNode<lingodb::ast::ColumnRefExpression>(@$, $ColId);}
    | ColId indirection {$$ = mkNode<lingodb::ast::ColumnRefExpression>(@$, $indirection, $ColId);} //TODO Add table name
    ;
//! TODO For what exactly is this here
indirection:
    indirection_el { $$=$1;}
    | indirection indirection_el {$$=$1;}
    ;
indirection_el:
    DOT attr_name {$$=$attr_name;}
    | DOT STAR //TODO make star
   //TODO | LB a_expr RB
   //TODO | LB opt_slice_bound ':' opt_slice_bound RB


/*****************************************************************************
 *
 *	target list for SELECT
 *
 *****************************************************************************/
opt_target_list:
    target_list { $$=$target_list;}
    | %empty
    ;
target_list:
    target_el { auto list = mkListShared<lingodb::ast::ParsedExpression>(); list.emplace_back($target_el); $$=list;}
    | target_list[list] COMMA target_el { $list.emplace_back($target_el); $$=$list;}
    ;
target_el:
    a_expr AS ColLabel {}
    | a_expr BareColLabel {}
    | a_expr { $$=$a_expr;}
    | STAR {  $$ =mkNode<lingodb::ast::StarExpression>(@$,"");  }
    ;



/*
 * Name classification hierarchy.
 *
 * IDENT is the lexeme returned by the lexer for identifiers that match
 * no known keyword.  In most cases, we can accept certain keywords as
 * names, not only IDENTs.	We prefer to accept as many such keywords
 * as possible to minimize the impact of "reserved words" on programmers.
 * So, we divide names into several possible classes.  The classification
 * is chosen in part to make keywords acceptable as names wherever possible.
 */

/* Column identifier --- names that can be column, table, etc names.
 */
 
ColId:
    IDENTIFIER {$$=$1;}
   //TODO | unreserved_keyword
   //TODO | col_name_keyword
   ;
/* Column label --- allowed labels in "AS" clauses.
 * This presently includes *all* Postgres keywords.
 */
ColLabel:
    IDENTIFIER									{ $$=$1; }
	//TODO | unreserved_keyword					{ }
	//TODO | col_name_keyword						{ }
	//TODO | type_func_name_keyword				{ }
	//TODO | reserved_keyword						{ }
	;
/* Bare column label --- names that can be column labels without writing "AS".
 * This classification is orthogonal to the other keyword categories.
 */
BareColLabel:
    IDENTIFIER								{  }
	//TODO | bare_label_keyword					{  }
	;








/*****************************************************************************
 *
 *	Names and constants
 *
 *****************************************************************************/
 //TODO Add missinge names and constants for instance qualified_name
qualified_name_list: 
    qualified_name 
    | qualified_name_list COMMA qualified_name
    ;
/*
 * Postgres
 * The production for a qualified relation name has to exactly match the
 * production for a qualified func_name, because in a FROM clause we cannot
 * tell which we are parsing until we see what comes after it ('(' for a
 * func_name, something else for a relation). Therefore we allow 'indirection'
 * which may contain subscripts, and reject that case in the C code.
 */
qualified_name:
    ColId { $$=$1;}
    | ColId indirection
    ;
name_list:
    name { auto t = mkList<std::string>(); t.emplace_back($name); $$=t;}
    | name_list[list] COMMA name {$list.emplace_back($name); $$=$list;}
    ;
name: ColId {$$=$1;};
attr_name: ColLabel {$$=$1;};

/*
 * Constants
 */
 //TODO Add missing AexprConst rules
AexprConst: 
    Iconst { $$=$1;}
;
Iconst:	
    ICONST	{ auto t = mkNode<lingodb::ast::ConstantExpression>(@$); t->iVal=$1; $$=t;  };
// ALLL
// stmt:
//     SelectStatement
//     {
//         drv.result = $SelectStatement;
//     }
//  ;
// SelectStatement:
//     select_no_parens
//     {
//         $$ = mkNode<nodes::SelectStatement>(@$, $select_no_parens);
//     }
//      ;
// select_no_parens:
//     simple_select
//     {
//         $$ = mkNode<nodes::Query>(@$, $simple_select);
//     }
// ;
// simple_select:
//     SELECT target_list from_clause
//     {
//         auto expr = mkNode<nodes::SelectExpr>(@$);
//         expr->from_clause = $from_clause;
//         expr->target_list = $target_list;
//         $$ = expr;
//     }
// ;

// target_list:
//    target_element {$$ = mkNode<nodes::RowExpr>(@$, $target_element);}
//  | target_list[list] COMMA target_element {}
//  ;
// target_element:
//     qualified_name { $$ = $qualified_name;}
//   | STAR {}
//  ;

// from_clause:
//     FROM from_list { $$ = $from_list;}

//   | %empty {}
// ;

// from_list:
//     table_ref {$$ = mkList<nodes::RelExpression>(@$); $$.emplace_back($table_ref); }
//    | from_list[list] COMMA table_ref {$list.emplace_back($table_ref); $$=$list;}
//  ;
// table_ref:
//     qualified_name[table_name]
//     {
//         auto node = mkNode<nodes::TableName>(@$, $table_name);
//         $$ = node;
//     }
// ;
// qualified_name:
//     qualifier_list[list]
// 	{
// 	    auto last = std::move($list.back());
//         $list.pop_back();
//         $$ = mkNode<nodes::QualifiedName>(@$, std::move(last));
//         $$->qualifier = $list;
// 	}
//  ;

//  qualifier_list:
//      IDENTIFIER[name]						{  auto l = mkList<std::string>(@$); l.emplace_back($name); $$ = l;}
//   |  qualifier_list[qn] DOT IDENTIFIER[name]			{ $qn.push_back($name); $$ = $qn;  }
//   ;

// where_clause:
//     WHERE bool_value_expr {}
// ;
// bool_value_expr:
//     bool_predicand {}
// ;
// bool_predicand:
//     common_value_expr {}
// ;
// common_value_expr:
//     IDENTIFIER

%%
void
lingodb::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
