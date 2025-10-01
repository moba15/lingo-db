// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

/**
 ** \file /home/bachmaier/projects/lingo-db/include/lingodb/compiler/frontend/generated/parser.hpp
 ** Define the  lingodb ::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_HOME_BACHMAIER_PROJECTS_LINGO_DB_INCLUDE_LINGODB_COMPILER_FRONTEND_GENERATED_PARSER_HPP_INCLUDED
#define YY_YY_HOME_BACHMAIER_PROJECTS_LINGO_DB_INCLUDE_LINGODB_COMPILER_FRONTEND_GENERATED_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 12 "/home/bachmaier/projects/lingo-db/src/compiler/frontend/parser.y"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "lingodb/compiler/frontend/ast/aggregation_node.h"
#include "lingodb/compiler/frontend/ast/constant_value.h"
#include "lingodb/compiler/frontend/ast/constraint.h"
#include "lingodb/compiler/frontend/ast/copy_node.h"
#include "lingodb/compiler/frontend/ast/create_node.h"
#include "lingodb/compiler/frontend/ast/extend_node.h"
#include "lingodb/compiler/frontend/ast/insert_node.h"
#include "lingodb/compiler/frontend/ast/parsed_expression.h"
#include "lingodb/compiler/frontend/ast/query_node.h"
#include "lingodb/compiler/frontend/ast/result_modifier.h"
#include "lingodb/compiler/frontend/ast/set_node.h"
#include "lingodb/compiler/frontend/ast/table_producer.h"
#include "lingodb/compiler/frontend/ast/tableref.h"
#include "lingodb/compiler/frontend/frontend_error.h"
class Driver;

#line 73 "/home/bachmaier/projects/lingo-db/include/lingodb/compiler/frontend/generated/parser.hpp"

#include <cassert>
#include <cstdlib> // std::abort
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#if defined __cplusplus
#define YY_CPLUSPLUS __cplusplus
#else
#define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
#define YY_MOVE std::move
#define YY_MOVE_OR_COPY move
#define YY_MOVE_REF(Type) Type&&
#define YY_RVREF(Type) Type&&
#define YY_COPY(Type) Type
#else
#define YY_MOVE
#define YY_MOVE_OR_COPY copy
#define YY_MOVE_REF(Type) Type&
#define YY_RVREF(Type) const Type&
#define YY_COPY(Type) const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
#define YY_NOEXCEPT noexcept
#define YY_NOTHROW
#else
#define YY_NOEXCEPT
#define YY_NOTHROW throw()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
#define YY_CONSTEXPR constexpr
#else
#define YY_CONSTEXPR
#endif
#include "location.hh"
#include <typeinfo>
#ifndef YY_ASSERT
#include <cassert>
#define YY_ASSERT assert
#endif

#ifndef YY_ATTRIBUTE_PURE
#if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#define YY_ATTRIBUTE_PURE __attribute__((__pure__))
#else
#define YY_ATTRIBUTE_PURE
#endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
#if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#define YY_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define YY_ATTRIBUTE_UNUSED
#endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#define YY_USE(E) ((void) (E))
#else
#define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && !defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
#if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
   _Pragma("GCC diagnostic push")           \
      _Pragma("GCC diagnostic ignored \"-Wuninitialized\"")
#else
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                 \
   _Pragma("GCC diagnostic push")                           \
      _Pragma("GCC diagnostic ignored \"-Wuninitialized\"") \
         _Pragma("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
#endif
#define YY_IGNORE_MAYBE_UNINITIALIZED_END \
   _Pragma("GCC diagnostic pop")
#else
#define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
#define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && !defined __ICC && 6 <= __GNUC__
#define YY_IGNORE_USELESS_CAST_BEGIN \
   _Pragma("GCC diagnostic push")    \
      _Pragma("GCC diagnostic ignored \"-Wuseless-cast\"")
#define YY_IGNORE_USELESS_CAST_END \
   _Pragma("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
#define YY_IGNORE_USELESS_CAST_BEGIN
#define YY_IGNORE_USELESS_CAST_END
#endif

#ifndef YY_CAST
#ifdef __cplusplus
#define YY_CAST(Type, Val) static_cast<Type>(Val)
#define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type>(Val)
#else
#define YY_CAST(Type, Val) ((Type) (Val))
#define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#endif
#endif
#ifndef YY_NULLPTR
#if defined __cplusplus
#if 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#else
#define YY_NULLPTR ((void*) 0)
#endif
#endif

/* Debug traces.  */
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#line 5 "/home/bachmaier/projects/lingo-db/src/compiler/frontend/parser.y"
namespace lingodb {
#line 214 "/home/bachmaier/projects/lingo-db/include/lingodb/compiler/frontend/generated/parser.hpp"

/// A Bison parser.
class parser {
   public:
#ifdef YYSTYPE
#ifdef __GNUC__
#pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
#endif
   typedef YYSTYPE value_type;
#else
   /// A buffer to store and retrieve objects.
   ///
   /// Sort of a variant, but does not keep track of the nature
   /// of the stored data, since that knowledge is available
   /// via the current parser state.
   class value_type {
      public:
      /// Type of *this.
      typedef value_type self_type;

      /// Empty construction.
      value_type() YY_NOEXCEPT
         : yyraw_(),
           yytypeid_(YY_NULLPTR) {}

      /// Construct and fill.
      template <typename T>
      value_type(YY_RVREF(T) t)
         : yytypeid_(&typeid(T)) {
         YY_ASSERT(sizeof(T) <= size);
         new (yyas_<T>()) T(YY_MOVE(t));
      }

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      value_type(const self_type&) = delete;
      /// Non copyable.
      self_type& operator=(const self_type&) = delete;
#endif

      /// Destruction, allowed only if empty.
      ~value_type() YY_NOEXCEPT {
         YY_ASSERT(!yytypeid_);
      }

#if 201103L <= YY_CPLUSPLUS
      /// Instantiate a \a T in here from \a t.
      template <typename T, typename... U>
      T&
      emplace(U&&... u) {
         YY_ASSERT(!yytypeid_);
         YY_ASSERT(sizeof(T) <= size);
         yytypeid_ = &typeid(T);
         return *new (yyas_<T>()) T(std::forward<U>(u)...);
      }
#else
      /// Instantiate an empty \a T in here.
      template <typename T>
      T&
      emplace() {
         YY_ASSERT(!yytypeid_);
         YY_ASSERT(sizeof(T) <= size);
         yytypeid_ = &typeid(T);
         return *new (yyas_<T>()) T();
      }

      /// Instantiate a \a T in here from \a t.
      template <typename T>
      T&
      emplace(const T& t) {
         YY_ASSERT(!yytypeid_);
         YY_ASSERT(sizeof(T) <= size);
         yytypeid_ = &typeid(T);
         return *new (yyas_<T>()) T(t);
      }
#endif

      /// Instantiate an empty \a T in here.
      /// Obsolete, use emplace.
      template <typename T>
      T&
      build() {
         return emplace<T>();
      }

      /// Instantiate a \a T in here from \a t.
      /// Obsolete, use emplace.
      template <typename T>
      T&
      build(const T& t) {
         return emplace<T>(t);
      }

      /// Accessor to a built \a T.
      template <typename T>
      T&
      as() YY_NOEXCEPT {
         YY_ASSERT(yytypeid_);
         YY_ASSERT(*yytypeid_ == typeid(T));
         YY_ASSERT(sizeof(T) <= size);
         return *yyas_<T>();
      }

      /// Const accessor to a built \a T (for %printer).
      template <typename T>
      const T&
      as() const YY_NOEXCEPT {
         YY_ASSERT(yytypeid_);
         YY_ASSERT(*yytypeid_ == typeid(T));
         YY_ASSERT(sizeof(T) <= size);
         return *yyas_<T>();
      }

      /// Swap the content with \a that, of same type.
      ///
      /// Both variants must be built beforehand, because swapping the actual
      /// data requires reading it (with as()), and this is not possible on
      /// unconstructed variants: it would require some dynamic testing, which
      /// should not be the variant's responsibility.
      /// Swapping between built and (possibly) non-built is done with
      /// self_type::move ().
      template <typename T>
      void
      swap(self_type& that) YY_NOEXCEPT {
         YY_ASSERT(yytypeid_);
         YY_ASSERT(*yytypeid_ == *that.yytypeid_);
         std::swap(as<T>(), that.as<T>());
      }

      /// Move the content of \a that to this.
      ///
      /// Destroys \a that.
      template <typename T>
      void
      move(self_type& that) {
#if 201103L <= YY_CPLUSPLUS
         emplace<T>(std::move(that.as<T>()));
#else
         emplace<T>();
         swap<T>(that);
#endif
         that.destroy<T>();
      }

#if 201103L <= YY_CPLUSPLUS
      /// Move the content of \a that to this.
      template <typename T>
      void
      move(self_type&& that) {
         emplace<T>(std::move(that.as<T>()));
         that.destroy<T>();
      }
#endif

      /// Copy the content of \a that to this.
      template <typename T>
      void
      copy(const self_type& that) {
         emplace<T>(that.as<T>());
      }

      /// Destroy the stored \a T.
      template <typename T>
      void
      destroy() {
         as<T>().~T();
         yytypeid_ = YY_NULLPTR;
      }

      private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      value_type(const self_type&);
      /// Non copyable.
      self_type& operator=(const self_type&);
#endif

      /// Accessor to raw memory as \a T.
      template <typename T>
      T*
      yyas_() YY_NOEXCEPT {
         void* yyp = yyraw_;
         return static_cast<T*>(yyp);
      }

      /// Const accessor to raw memory as \a T.
      template <typename T>
      const T*
      yyas_() const YY_NOEXCEPT {
         const void* yyp = yyraw_;
         return static_cast<const T*>(yyp);
      }

      /// An auxiliary type to compute the largest semantic type.
      union union_type {
         // distinct_clause
         // set_quantifier
         // opt_asymmetric
         // OptTemp
         // opt_varying
         char dummy1[sizeof(bool)];

         // ICONST
         char dummy2[sizeof(int)];

         // when_clause
         char dummy3[sizeof(lingodb::ast::CaseExpression::CaseCheck)];

         // basicComparisonType
         char dummy4[sizeof(lingodb::ast::ExpressionType)];

         // join_type
         char dummy5[sizeof(lingodb::ast::JoinType)];

         // Type
         // SimpleType
         // ConstTypename
         // Numeric_with_opt_lenghth
         // Numeric
         // Character
         // ConstCharacter
         // CharacterWithLength
         // CharacterWithoutLength
         // character
         // ConstDatetime
         // ConstInterval
         // Typename
         char dummy6[sizeof(lingodb::ast::LogicalTypeWithMods)];

         // opt_nulls_order
         char dummy7[sizeof(lingodb::ast::OrderByNullType)];

         // opt_asc_desc
         char dummy8[sizeof(lingodb::ast::OrderType)];

         // sub_type
         char dummy9[sizeof(lingodb::ast::SubqueryType)];

         // join_qual
         char dummy10[sizeof(lingodb::ast::jointCondOrUsingCols)];

         // opt_interval
         char dummy11[sizeof(std::optional<lingodb::ast::SQLAbstractLogicalType>)];

         // opt_sort_clause
         char dummy12[sizeof(std::optional<std::shared_ptr<lingodb::ast::OrderByModifier>>)];

         // case_arg
         char dummy13[sizeof(std::optional<std::shared_ptr<lingodb::ast::ParsedExpression>>)];

         // opt_select_limit
         char dummy14[sizeof(std::optional<std::shared_ptr<lingodb::ast::ResultModifier>>)];

         // copy_opt_item
         char dummy15[sizeof(std::pair<std::string, std::string>)];

         // alias_clause
         // opt_alias_clause
         char dummy16[sizeof(std::pair<std::string, std::vector<std::string>>)];

         // extract_arg
         char dummy17[sizeof(std::shared_ptr<ast::ConstantExpression>)];

         // CopyStmt
         char dummy18[sizeof(std::shared_ptr<ast::CopyNode>)];

         // VariableSetStmt
         // set_rest
         // generic_set
         // set_rest_more
         char dummy19[sizeof(std::shared_ptr<ast::SetVariableStatement>)];

         // agg_expr
         char dummy20[sizeof(std::shared_ptr<lingodb::ast::AggregationNode>)];

         // toplevel_stmt
         // stmt
         char dummy21[sizeof(std::shared_ptr<lingodb::ast::AstNode>)];

         // and_a_expr
         // or_a_expr
         char dummy22[sizeof(std::shared_ptr<lingodb::ast::ConjunctionExpression>)];

         // ColConstraint
         // ColConstraintElem
         // ConstraintElem
         char dummy23[sizeof(std::shared_ptr<lingodb::ast::Constraint>)];

         // CreateStmt
         char dummy24[sizeof(std::shared_ptr<lingodb::ast::CreateNode>)];

         // from_list
         char dummy25[sizeof(std::shared_ptr<lingodb::ast::CrossProductRef>)];

         // values_clause
         char dummy26[sizeof(std::shared_ptr<lingodb::ast::ExpressionListRef>)];

         // func_application
         // func_expr
         // func_expr_common_subexpr
         char dummy27[sizeof(std::shared_ptr<lingodb::ast::FunctionExpression>)];

         // group_clause
         // group_clause_with_alias
         char dummy28[sizeof(std::shared_ptr<lingodb::ast::GroupByNode>)];

         // InsertStmt
         // insert_rest
         char dummy29[sizeof(std::shared_ptr<lingodb::ast::InsertNode>)];

         // select_limit
         // limit_clause
         char dummy30[sizeof(std::shared_ptr<lingodb::ast::LimitModifier>)];

         // sortby
         char dummy31[sizeof(std::shared_ptr<lingodb::ast::OrderByElement>)];

         // sort_clause
         char dummy32[sizeof(std::shared_ptr<lingodb::ast::OrderByModifier>)];

         // var_value
         // opt_boolean_or_string
         // NonReservedWord_or_Sconst
         // group_by_item
         // group_by_item_with_alias
         // having_clause
         // offset_clause
         // select_limit_value
         // select_offset_value
         // where_clause
         // a_expr
         // b_expr
         // c_expr
         // case_expr
         // case_default
         // columnref
         // func_arg_expr
         // cast_expr
         // indirection
         // indirection_el
         // target_el
         // NumericOnly
         // AexprConst
         // Iconst
         // SignedIconst
         // Fconst
         // Sconst
         // Bconst
         char dummy33[sizeof(std::shared_ptr<lingodb::ast::ParsedExpression>)];

         // pipe_start
         // pipe_operator
         char dummy34[sizeof(std::shared_ptr<lingodb::ast::PipeOperator>)];

         // select_with_parens
         // select_no_parens
         // select_clause
         // simple_select
         // with_clause
         // cte_list
         // common_table_expr
         char dummy35[sizeof(std::shared_ptr<lingodb::ast::QueryNode>)];

         // set_list
         char dummy36[sizeof(std::shared_ptr<lingodb::ast::SetColumnExpression>)];

         // TableElement
         // TableConstraint
         // columnElement
         char dummy37[sizeof(std::shared_ptr<lingodb::ast::TableElement>)];

         // SelectStmt
         // PipeSQLStmt
         // classic_select_and_pipe_sql_with_parens
         // pipe_sql_with_parens
         // pipe_sql_no_parens
         // pipe_or_select_clause
         // PreparableStmt
         char dummy38[sizeof(std::shared_ptr<lingodb::ast::TableProducer>)];

         // opt_from_clause
         // from_clause
         // table_ref
         // joined_table
         char dummy39[sizeof(std::shared_ptr<lingodb::ast::TableRef>)];

         // opt_target_list
         char dummy40[sizeof(std::shared_ptr<lingodb::ast::TargetList>)];

         // type_modifier
         char dummy41[sizeof(std::shared_ptr<lingodb::ast::Value>)];

         // window_func_expr
         // over_clause
         // window_specification
         char dummy42[sizeof(std::shared_ptr<lingodb::ast::WindowExpression>)];

         // opt_frame_clause
         // frame_extent
         // frame_bound
         char dummy43[sizeof(std::shared_ptr<lingodb::ast::WindowFrame>)];

         // FCONST
         // IDENT
         // "identifier"
         // STRING_VALUE
         // "bit_string"
         // "hex_string"
         // "nat_string"
         // "%"
         // "+"
         // "/"
         // "*"
         // "-"
         // "="
         // "<>"
         // "<="
         // "<"
         // ">="
         // ">"
         // UIDENT
         // SCONST
         // USCONST
         // BCONST
         // XCONST
         // Op
         // TYPECAST
         // ABORT_P
         // ABSENT
         // ABSOLUTE_P
         // ACCESS
         // ACTION
         // ADD_P
         // ADMIN
         // AFTER
         // AGGREGATE
         // ALL
         // ALSO
         // ALTER
         // ALWAYS
         // ANALYSE
         // ANALYZE
         // AND
         // ANY
         // ARRAY
         // AS
         // ASC
         // ASENSITIVE
         // ASSERTION
         // ASSIGNMENT
         // ASYMMETRIC
         // ATOMIC
         // AT
         // ATTACH
         // ATTRIBUTE
         // AUTHORIZATION
         // BACKWARD
         // BEFORE
         // BEGIN_P
         // BETWEEN
         // BIGINT
         // BINARY
         // BIT
         // BOOLEAN_P
         // BOTH
         // BREADTH
         // BY
         // CACHE
         // CALL
         // CALLED
         // CASCADE
         // CASCADED
         // CASE
         // CAST
         // CATALOG_P
         // CHAIN
         // CHAR_P
         // CHARACTER
         // CHARACTERISTICS
         // CHECK
         // CHECKPOINT
         // CLASS
         // CLOSE
         // CLUSTER
         // COALESCE
         // COLLATE
         // COLLATION
         // COLUMN
         // COLUMNS
         // COMMENT
         // COMMENTS
         // COMMIT
         // COMMITTED
         // COMPRESSION
         // CONCURRENTLY
         // CONDITIONAL
         // CONFIGURATION
         // CONFLICT
         // CONNECTION
         // CONSTRAINT
         // CONSTRAINTS
         // CONTENT_P
         // CONTINUE_P
         // CONVERSION_P
         // COPY
         // COST
         // CREATE
         // CROSS
         // CSV
         // CUBE
         // CURRENT_P
         // CURRENT_CATALOG
         // CURRENT_DATE
         // CURRENT_ROLE
         // CURRENT_SCHEMA
         // CURRENT_TIME
         // CURRENT_TIMESTAMP
         // CURRENT_USER
         // CURSOR
         // CYCLE
         // DATA_P
         // DATABASE
         // DAY_P
         // DEALLOCATE
         // DEC
         // DECIMAL_P
         // DECLARE
         // DEFAULT
         // DEFAULTS
         // DATE_P
         // DEFERRABLE
         // DEFERRED
         // DEFINER
         // DELETE_P
         // DELIMITER
         // DELIMITERS
         // DEPENDS
         // DEPTH
         // DESC
         // DETACH
         // DICTIONARY
         // DISABLE_P
         // DISCARD
         // DISTINCT
         // DO
         // DOCUMENT_P
         // DOMAIN_P
         // DOUBLE_P
         // DROP
         // EACH
         // ELSE
         // EMPTY_P
         // ENABLE_P
         // ENCODING
         // ENCRYPTED
         // END_P
         // ENFORCED
         // ENUM_P
         // ERROR_P
         // ESCAPE
         // EVENT
         // EXCEPT
         // EXCLUDE
         // EXCLUDING
         // EXCLUSIVE
         // EXECUTE
         // EXISTS
         // EXPLAIN
         // EXPRESSION
         // EXTENSION
         // EXTERNAL
         // EXTRACT
         // FALSE_P
         // FAMILY
         // FETCH
         // FILTER
         // FINALIZE
         // FIRST_P
         // FLOAT_P
         // FOLLOWING
         // FOR
         // FORCE
         // FOREIGN
         // FORMAT
         // FORWARD
         // FREEZE
         // FROM
         // FULL
         // FUNCTION
         // FUNCTIONS
         // GENERATED
         // GLOBAL
         // GRANT
         // GRANTED
         // GREATEST
         // GROUP_P
         // GROUPING
         // GROUPS
         // HANDLER
         // HAVING
         // HEADER_P
         // HOLD
         // HOUR_P
         // IDENTITY_P
         // IF_P
         // ILIKE
         // IMMEDIATE
         // IMMUTABLE
         // IMPLICIT_P
         // IMPORT_P
         // IN_P
         // INCLUDE
         // INCLUDING
         // INCREMENT
         // INDENT
         // INDEX
         // INDEXES
         // INHERIT
         // INHERITS
         // INITIALLY
         // INLINE_P
         // INNER_P
         // INOUT
         // INPUT_P
         // INSENSITIVE
         // INSERT
         // INSTEAD
         // INT_P
         // INTEGER
         // INTERSECT
         // INTERVAL
         // INTO
         // INVOKER
         // IS
         // ISNULL
         // ISOLATION
         // JOIN
         // JSON
         // JSON_ARRAY
         // JSON_ARRAYAGG
         // JSON_EXISTS
         // JSON_OBJECT
         // JSON_OBJECTAGG
         // JSON_QUERY
         // JSON_SCALAR
         // JSON_SERIALIZE
         // JSON_TABLE
         // JSON_VALUE
         // KEEP
         // KEY
         // KEYS
         // LABEL
         // LANGUAGE
         // LARGE_P
         // LAST_P
         // LATERAL_P
         // LEADING
         // LEAKPROOF
         // LEAST
         // LEFT
         // LEVEL
         // LIKE
         // LIMIT
         // LISTEN
         // LOAD
         // LOCAL
         // LOCALTIME
         // LOCALTIMESTAMP
         // LOCATION
         // LOCK_P
         // LOCKED
         // LOGGED
         // MAPPING
         // MATCH
         // MATCHED
         // MATERIALIZED
         // MAXVALUE
         // MERGE
         // MERGE_ACTION
         // METHOD
         // MINUTE_P
         // MINVALUE
         // MODE
         // MONTH_P
         // MOVE
         // NAME_P
         // NAMES
         // NATIONAL
         // NATURAL
         // NCHAR
         // NESTED
         // NEW
         // NEXT
         // NFC
         // NFD
         // NFKC
         // NFKD
         // NO
         // NONE
         // NORMALIZE
         // NORMALIZED
         // NOT
         // NOTHING
         // NOTIFY
         // NOTNULL
         // NOWAIT
         // NULL_P
         // NULLIF
         // NULLS_P
         // NUMERIC
         // OBJECT_P
         // OBJECTS_P
         // OF
         // OFF
         // OFFSET
         // OIDS
         // OLD
         // OMIT
         // ON
         // ONLY
         // OPERATOR
         // OPTION
         // OPTIONS
         // OR
         // ORDER
         // ORDINALITY
         // OTHERS
         // OUT_P
         // OUTER_P
         // OVER
         // OVERLAPS
         // OVERLAY
         // OVERRIDING
         // OWNED
         // OWNER
         // PARALLEL
         // PARAMETER
         // PARSER
         // PARTIAL
         // PARTITION
         // PASSING
         // PASSWORD
         // PATH
         // PERIOD
         // PLACING
         // PLAN
         // PLANS
         // POLICY
         // POSITION
         // PRECEDING
         // PRECISION
         // PRESERVE
         // PREPARE
         // PREPARED
         // PRIMARY
         // PRIOR
         // PRIVILEGES
         // PROCEDURAL
         // PROCEDURE
         // PROCEDURES
         // PROGRAM
         // PUBLICATION
         // QUOTES
         // EXTEND
         // RANGE
         // READ
         // REAL
         // REASSIGN
         // RECURSIVE
         // REF_P
         // REFERENCES
         // REFERENCING
         // REFRESH
         // REINDEX
         // RELATIVE_P
         // RELEASE
         // RENAME
         // REPEATABLE
         // REPLACE
         // REPLICA
         // RESET
         // RESTART
         // RESTRICT
         // RETURN
         // RETURNING
         // RETURNS
         // REVOKE
         // RIGHT
         // ROLE
         // ROLLBACK
         // ROLLUP
         // ROUTINE
         // ROUTINES
         // ROW
         // ROWS
         // RULE
         // SAVEPOINT
         // SCALAR
         // SCHEMA
         // SCHEMAS
         // SCROLL
         // SEARCH
         // SECOND_P
         // SECURITY
         // SELECT
         // SEQUENCE
         // SEQUENCES
         // SERIALIZABLE
         // SERVER
         // SESSION
         // SESSION_USER
         // SET
         // SETS
         // SETOF
         // SHARE
         // SHOW
         // SIMILAR
         // SIMPLE
         // SKIP
         // SMALLINT
         // SNAPSHOT
         // SOME
         // SOURCE
         // SQL_P
         // STABLE
         // STANDALONE_P
         // START
         // STATEMENT
         // STATISTICS
         // STDIN
         // STDOUT
         // STORAGE
         // STORED
         // STRICT_P
         // STRING_P
         // STRIP_P
         // SUBSCRIPTION
         // SUBSTRING
         // SUPPORT
         // SYMMETRIC
         // SYSID
         // SYSTEM_P
         // SYSTEM_USER
         // TABLE
         // TABLES
         // TABLESAMPLE
         // TABLESPACE
         // TARGET
         // TEMP
         // TEMPLATE
         // TEMPORARY
         // TEXT_P
         // THEN
         // TIES
         // TIME
         // TIMESTAMP
         // TO
         // TRAILING
         // TRANSACTION
         // TRANSFORM
         // TREAT
         // TRIGGER
         // TRIM
         // TRUE_P
         // TRUNCATE
         // TRUSTED
         // TYPE_P
         // TYPES_P
         // UESCAPE
         // UNBOUNDED
         // UNCONDITIONAL
         // UNCOMMITTED
         // UNENCRYPTED
         // UNION
         // UNIQUE
         // UNKNOWN
         // UNLISTEN
         // UNLOGGED
         // UNTIL
         // UPDATE
         // USER
         // USING
         // VACUUM
         // VALID
         // VALIDATE
         // VALIDATOR
         // VALUE_P
         // VALUES
         // VARCHAR
         // VARIADIC
         // VARYING
         // VERBOSE
         // VERSION_P
         // VIEW
         // VIEWS
         // VIRTUAL
         // VOLATILE
         // WHEN
         // WHERE
         // WHITESPACE_P
         // WINDOW
         // WITH
         // WITHIN
         // WITHOUT
         // WORK
         // WRAPPER
         // WRITE
         // XML_P
         // XMLATTRIBUTES
         // XMLCONCAT
         // XMLELEMENT
         // XMLEXISTS
         // XMLFOREST
         // XMLNAMESPACES
         // XMLPARSE
         // XMLPI
         // XMLROOT
         // XMLSERIALIZE
         // XMLTABLE
         // YEAR_P
         // YES_P
         // ZONE
         // copy_file_name
         // copy_delimiter
         // var_name
         // NonReservedWord
         // relation_expr
         // any_operator
         // qual_Op
         // all_Op
         // ColId
         // type_function_name
         // type_func_name_keyword
         // ColLabel
         // BareColLabel
         // reserved_keyword
         // unreserved_keyword
         // col_name_keyword
         // qualified_name
         // name
         // attr_name
         // func_name
         // insert_target
         // insert_column_item
         char dummy44[sizeof(std::string)];

         // in_expr
         char dummy45[sizeof(std::variant<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>, std::shared_ptr<lingodb::ast::SubqueryExpression>>)];

         // when_clause_list
         char dummy46[sizeof(std::vector<lingodb::ast::CaseExpression::CaseCheck>)];

         // copy_options
         // copy_opt_list
         char dummy47[sizeof(std::vector<std::pair<std::string, std::string>>)];

         // stmtmulti
         char dummy48[sizeof(std::vector<std::shared_ptr<lingodb::ast::AstNode>>)];

         // ColQualList
         char dummy49[sizeof(std::vector<std::shared_ptr<lingodb::ast::Constraint>>)];

         // func_expr_list
         char dummy50[sizeof(std::vector<std::shared_ptr<lingodb::ast::FunctionExpression>>)];

         // sortby_list
         char dummy51[sizeof(std::vector<std::shared_ptr<lingodb::ast::OrderByElement>>)];

         // var_list
         // group_by_list
         // group_by_list_with_alias
         // rollup_clause
         // columnref_list
         // func_arg_list_opt
         // func_arg_list
         // expr_list
         // expr_list_with_alias
         // extract_list
         // substr_list
         // target_list
         // opt_partition_clause
         char dummy52[sizeof(std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>)];

         // OptTableElementList
         // TableElementList
         char dummy53[sizeof(std::vector<std::shared_ptr<lingodb::ast::TableElement>>)];

         // opt_type_modifiers
         // type_modifiers
         char dummy54[sizeof(std::vector<std::shared_ptr<lingodb::ast::Value>>)];

         // opt_name_list
         // name_list
         // insert_column_list
         char dummy55[sizeof(std::vector<std::string>)];

         // "integer_value"
         char dummy56[sizeof(uint64_t)];
      };

      /// The size of the largest semantic type.
      enum { size = sizeof(union_type) };

      /// A buffer to store semantic values.
      union {
         /// Strongest alignment constraints.
         long double yyalign_me_;
         /// A buffer large enough to store any of the semantic values.
         char yyraw_[size];
      };

      /// Whether the content is built: if defined, the name of the stored type.
      const std::type_info* yytypeid_;
   };

#endif
   /// Backward compatibility (Bison 3.8).
   typedef value_type semantic_type;

   /// Symbol locations.
   typedef location location_type;

   /// Syntax errors thrown from user actions.
   struct syntax_error : std::runtime_error {
      syntax_error(const location_type& l, const std::string& m)
         : std::runtime_error(m), location(l) {}

      syntax_error(const syntax_error& s)
         : std::runtime_error(s.what()), location(s.location) {}

      ~syntax_error() YY_NOEXCEPT YY_NOTHROW;

      location_type location;
   };

   /// Token kinds.
   struct token {
      enum token_kind_type {
         TOK_YYEMPTY = -2,
         TOK_YYEOF = 0, // "end of file"
         TOK_YYerror = 1, // error
         TOK_YYUNDEF = 2, // "invalid token"
         TOK_ICONST = 3, // ICONST
         TOK_INTEGER_VALUE = 4, // "integer_value"
         TOK_FCONST = 5, // FCONST
         TOK_IDENT = 6, // IDENT
         TOK_IDENTIFIER = 7, // "identifier"
         TOK_STRING_VALUE = 8, // STRING_VALUE
         TOK_BIT_VALUE = 9, // "bit_string"
         TOK_HEX_VALUE = 10, // "hex_string"
         TOK_NATIONAL_VALUE = 11, // "nat_string"
         TOK_LP = 12, // "("
         TOK_RP = 13, // ")"
         TOK_LB = 14, // "["
         TOK_RB = 15, // "]"
         TOK_DOT = 16, // "."
         TOK_PERCENT = 17, // "%"
         TOK_COMMA = 18, // ","
         TOK_SEMICOLON = 19, // ";"
         TOK_PLUS = 20, // "+"
         TOK_SLASH = 21, // "/"
         TOK_STAR = 22, // "*"
         TOK_MINUS = 23, // "-"
         TOK_EQUAL = 24, // "="
         TOK_NOT_EQUAL = 25, // "<>"
         TOK_LESS_EQUAL = 26, // "<="
         TOK_LESS = 27, // "<"
         TOK_GREATER_EQUAL = 28, // ">="
         TOK_GREATER = 29, // ">"
         TOK_HAT = 30, // "^"
         TOK_QUOTE = 31, // "'"
         TOK_PIPE = 32, // "|>"
         TOK_UIDENT = 33, // UIDENT
         TOK_SCONST = 34, // SCONST
         TOK_USCONST = 35, // USCONST
         TOK_BCONST = 36, // BCONST
         TOK_XCONST = 37, // XCONST
         TOK_Op = 38, // Op
         TOK_PARAM = 39, // PARAM
         TOK_TYPECAST = 40, // TYPECAST
         TOK_DOT_DOT = 41, // DOT_DOT
         TOK_COLON_EQUALS = 42, // COLON_EQUALS
         TOK_ABORT_P = 43, // ABORT_P
         TOK_ABSENT = 44, // ABSENT
         TOK_ABSOLUTE_P = 45, // ABSOLUTE_P
         TOK_ACCESS = 46, // ACCESS
         TOK_ACTION = 47, // ACTION
         TOK_ADD_P = 48, // ADD_P
         TOK_ADMIN = 49, // ADMIN
         TOK_AFTER = 50, // AFTER
         TOK_AGGREGATE = 51, // AGGREGATE
         TOK_ALL = 52, // ALL
         TOK_ALSO = 53, // ALSO
         TOK_ALTER = 54, // ALTER
         TOK_ALWAYS = 55, // ALWAYS
         TOK_ANALYSE = 56, // ANALYSE
         TOK_ANALYZE = 57, // ANALYZE
         TOK_AND = 58, // AND
         TOK_ANY = 59, // ANY
         TOK_ARRAY = 60, // ARRAY
         TOK_AS = 61, // AS
         TOK_ASC = 62, // ASC
         TOK_ASENSITIVE = 63, // ASENSITIVE
         TOK_ASSERTION = 64, // ASSERTION
         TOK_ASSIGNMENT = 65, // ASSIGNMENT
         TOK_ASYMMETRIC = 66, // ASYMMETRIC
         TOK_ATOMIC = 67, // ATOMIC
         TOK_AT = 68, // AT
         TOK_ATTACH = 69, // ATTACH
         TOK_ATTRIBUTE = 70, // ATTRIBUTE
         TOK_AUTHORIZATION = 71, // AUTHORIZATION
         TOK_BACKWARD = 72, // BACKWARD
         TOK_BEFORE = 73, // BEFORE
         TOK_BEGIN_P = 74, // BEGIN_P
         TOK_BETWEEN = 75, // BETWEEN
         TOK_BIGINT = 76, // BIGINT
         TOK_BINARY = 77, // BINARY
         TOK_BIT = 78, // BIT
         TOK_BOOLEAN_P = 79, // BOOLEAN_P
         TOK_BOTH = 80, // BOTH
         TOK_BREADTH = 81, // BREADTH
         TOK_BY = 82, // BY
         TOK_CACHE = 83, // CACHE
         TOK_CALL = 84, // CALL
         TOK_CALLED = 85, // CALLED
         TOK_CASCADE = 86, // CASCADE
         TOK_CASCADED = 87, // CASCADED
         TOK_CASE = 88, // CASE
         TOK_CAST = 89, // CAST
         TOK_CATALOG_P = 90, // CATALOG_P
         TOK_CHAIN = 91, // CHAIN
         TOK_CHAR_P = 92, // CHAR_P
         TOK_CHARACTER = 93, // CHARACTER
         TOK_CHARACTERISTICS = 94, // CHARACTERISTICS
         TOK_CHECK = 95, // CHECK
         TOK_CHECKPOINT = 96, // CHECKPOINT
         TOK_CLASS = 97, // CLASS
         TOK_CLOSE = 98, // CLOSE
         TOK_CLUSTER = 99, // CLUSTER
         TOK_COALESCE = 100, // COALESCE
         TOK_COLLATE = 101, // COLLATE
         TOK_COLLATION = 102, // COLLATION
         TOK_COLUMN = 103, // COLUMN
         TOK_COLUMNS = 104, // COLUMNS
         TOK_COMMENT = 105, // COMMENT
         TOK_COMMENTS = 106, // COMMENTS
         TOK_COMMIT = 107, // COMMIT
         TOK_COMMITTED = 108, // COMMITTED
         TOK_COMPRESSION = 109, // COMPRESSION
         TOK_CONCURRENTLY = 110, // CONCURRENTLY
         TOK_CONDITIONAL = 111, // CONDITIONAL
         TOK_CONFIGURATION = 112, // CONFIGURATION
         TOK_CONFLICT = 113, // CONFLICT
         TOK_CONNECTION = 114, // CONNECTION
         TOK_CONSTRAINT = 115, // CONSTRAINT
         TOK_CONSTRAINTS = 116, // CONSTRAINTS
         TOK_CONTENT_P = 117, // CONTENT_P
         TOK_CONTINUE_P = 118, // CONTINUE_P
         TOK_CONVERSION_P = 119, // CONVERSION_P
         TOK_COPY = 120, // COPY
         TOK_COST = 121, // COST
         TOK_CREATE = 122, // CREATE
         TOK_CROSS = 123, // CROSS
         TOK_CSV = 124, // CSV
         TOK_CUBE = 125, // CUBE
         TOK_CURRENT_P = 126, // CURRENT_P
         TOK_CURRENT_CATALOG = 127, // CURRENT_CATALOG
         TOK_CURRENT_DATE = 128, // CURRENT_DATE
         TOK_CURRENT_ROLE = 129, // CURRENT_ROLE
         TOK_CURRENT_SCHEMA = 130, // CURRENT_SCHEMA
         TOK_CURRENT_TIME = 131, // CURRENT_TIME
         TOK_CURRENT_TIMESTAMP = 132, // CURRENT_TIMESTAMP
         TOK_CURRENT_USER = 133, // CURRENT_USER
         TOK_CURSOR = 134, // CURSOR
         TOK_CYCLE = 135, // CYCLE
         TOK_DATA_P = 136, // DATA_P
         TOK_DATABASE = 137, // DATABASE
         TOK_DAY_P = 138, // DAY_P
         TOK_DEALLOCATE = 139, // DEALLOCATE
         TOK_DEC = 140, // DEC
         TOK_DECIMAL_P = 141, // DECIMAL_P
         TOK_DECLARE = 142, // DECLARE
         TOK_DEFAULT = 143, // DEFAULT
         TOK_DEFAULTS = 144, // DEFAULTS
         TOK_DATE_P = 145, // DATE_P
         TOK_DEFERRABLE = 146, // DEFERRABLE
         TOK_DEFERRED = 147, // DEFERRED
         TOK_DEFINER = 148, // DEFINER
         TOK_DELETE_P = 149, // DELETE_P
         TOK_DELIMITER = 150, // DELIMITER
         TOK_DELIMITERS = 151, // DELIMITERS
         TOK_DEPENDS = 152, // DEPENDS
         TOK_DEPTH = 153, // DEPTH
         TOK_DESC = 154, // DESC
         TOK_DETACH = 155, // DETACH
         TOK_DICTIONARY = 156, // DICTIONARY
         TOK_DISABLE_P = 157, // DISABLE_P
         TOK_DISCARD = 158, // DISCARD
         TOK_DISTINCT = 159, // DISTINCT
         TOK_DO = 160, // DO
         TOK_DOCUMENT_P = 161, // DOCUMENT_P
         TOK_DOMAIN_P = 162, // DOMAIN_P
         TOK_DOUBLE_P = 163, // DOUBLE_P
         TOK_DROP = 164, // DROP
         TOK_EACH = 165, // EACH
         TOK_ELSE = 166, // ELSE
         TOK_EMPTY_P = 167, // EMPTY_P
         TOK_ENABLE_P = 168, // ENABLE_P
         TOK_ENCODING = 169, // ENCODING
         TOK_ENCRYPTED = 170, // ENCRYPTED
         TOK_END_P = 171, // END_P
         TOK_ENFORCED = 172, // ENFORCED
         TOK_ENUM_P = 173, // ENUM_P
         TOK_ERROR_P = 174, // ERROR_P
         TOK_ESCAPE = 175, // ESCAPE
         TOK_EVENT = 176, // EVENT
         TOK_EXCEPT = 177, // EXCEPT
         TOK_EXCLUDE = 178, // EXCLUDE
         TOK_EXCLUDING = 179, // EXCLUDING
         TOK_EXCLUSIVE = 180, // EXCLUSIVE
         TOK_EXECUTE = 181, // EXECUTE
         TOK_EXISTS = 182, // EXISTS
         TOK_EXPLAIN = 183, // EXPLAIN
         TOK_EXPRESSION = 184, // EXPRESSION
         TOK_EXTENSION = 185, // EXTENSION
         TOK_EXTERNAL = 186, // EXTERNAL
         TOK_EXTRACT = 187, // EXTRACT
         TOK_FALSE_P = 188, // FALSE_P
         TOK_FAMILY = 189, // FAMILY
         TOK_FETCH = 190, // FETCH
         TOK_FILTER = 191, // FILTER
         TOK_FINALIZE = 192, // FINALIZE
         TOK_FIRST_P = 193, // FIRST_P
         TOK_FLOAT_P = 194, // FLOAT_P
         TOK_FOLLOWING = 195, // FOLLOWING
         TOK_FOR = 196, // FOR
         TOK_FORCE = 197, // FORCE
         TOK_FOREIGN = 198, // FOREIGN
         TOK_FORMAT = 199, // FORMAT
         TOK_FORWARD = 200, // FORWARD
         TOK_FREEZE = 201, // FREEZE
         TOK_FROM = 202, // FROM
         TOK_FULL = 203, // FULL
         TOK_FUNCTION = 204, // FUNCTION
         TOK_FUNCTIONS = 205, // FUNCTIONS
         TOK_GENERATED = 206, // GENERATED
         TOK_GLOBAL = 207, // GLOBAL
         TOK_GRANT = 208, // GRANT
         TOK_GRANTED = 209, // GRANTED
         TOK_GREATEST = 210, // GREATEST
         TOK_GROUP_P = 211, // GROUP_P
         TOK_GROUPING = 212, // GROUPING
         TOK_GROUPS = 213, // GROUPS
         TOK_HANDLER = 214, // HANDLER
         TOK_HAVING = 215, // HAVING
         TOK_HEADER_P = 216, // HEADER_P
         TOK_HOLD = 217, // HOLD
         TOK_HOUR_P = 218, // HOUR_P
         TOK_IDENTITY_P = 219, // IDENTITY_P
         TOK_IF_P = 220, // IF_P
         TOK_ILIKE = 221, // ILIKE
         TOK_IMMEDIATE = 222, // IMMEDIATE
         TOK_IMMUTABLE = 223, // IMMUTABLE
         TOK_IMPLICIT_P = 224, // IMPLICIT_P
         TOK_IMPORT_P = 225, // IMPORT_P
         TOK_IN_P = 226, // IN_P
         TOK_INCLUDE = 227, // INCLUDE
         TOK_INCLUDING = 228, // INCLUDING
         TOK_INCREMENT = 229, // INCREMENT
         TOK_INDENT = 230, // INDENT
         TOK_INDEX = 231, // INDEX
         TOK_INDEXES = 232, // INDEXES
         TOK_INHERIT = 233, // INHERIT
         TOK_INHERITS = 234, // INHERITS
         TOK_INITIALLY = 235, // INITIALLY
         TOK_INLINE_P = 236, // INLINE_P
         TOK_INNER_P = 237, // INNER_P
         TOK_INOUT = 238, // INOUT
         TOK_INPUT_P = 239, // INPUT_P
         TOK_INSENSITIVE = 240, // INSENSITIVE
         TOK_INSERT = 241, // INSERT
         TOK_INSTEAD = 242, // INSTEAD
         TOK_INT_P = 243, // INT_P
         TOK_INTEGER = 244, // INTEGER
         TOK_INTERSECT = 245, // INTERSECT
         TOK_INTERVAL = 246, // INTERVAL
         TOK_INTO = 247, // INTO
         TOK_INVOKER = 248, // INVOKER
         TOK_IS = 249, // IS
         TOK_ISNULL = 250, // ISNULL
         TOK_ISOLATION = 251, // ISOLATION
         TOK_JOIN = 252, // JOIN
         TOK_JSON = 253, // JSON
         TOK_JSON_ARRAY = 254, // JSON_ARRAY
         TOK_JSON_ARRAYAGG = 255, // JSON_ARRAYAGG
         TOK_JSON_EXISTS = 256, // JSON_EXISTS
         TOK_JSON_OBJECT = 257, // JSON_OBJECT
         TOK_JSON_OBJECTAGG = 258, // JSON_OBJECTAGG
         TOK_JSON_QUERY = 259, // JSON_QUERY
         TOK_JSON_SCALAR = 260, // JSON_SCALAR
         TOK_JSON_SERIALIZE = 261, // JSON_SERIALIZE
         TOK_JSON_TABLE = 262, // JSON_TABLE
         TOK_JSON_VALUE = 263, // JSON_VALUE
         TOK_KEEP = 264, // KEEP
         TOK_KEY = 265, // KEY
         TOK_KEYS = 266, // KEYS
         TOK_LABEL = 267, // LABEL
         TOK_LANGUAGE = 268, // LANGUAGE
         TOK_LARGE_P = 269, // LARGE_P
         TOK_LAST_P = 270, // LAST_P
         TOK_LATERAL_P = 271, // LATERAL_P
         TOK_LEADING = 272, // LEADING
         TOK_LEAKPROOF = 273, // LEAKPROOF
         TOK_LEAST = 274, // LEAST
         TOK_LEFT = 275, // LEFT
         TOK_LEVEL = 276, // LEVEL
         TOK_LIKE = 277, // LIKE
         TOK_LIMIT = 278, // LIMIT
         TOK_LISTEN = 279, // LISTEN
         TOK_LOAD = 280, // LOAD
         TOK_LOCAL = 281, // LOCAL
         TOK_LOCALTIME = 282, // LOCALTIME
         TOK_LOCALTIMESTAMP = 283, // LOCALTIMESTAMP
         TOK_LOCATION = 284, // LOCATION
         TOK_LOCK_P = 285, // LOCK_P
         TOK_LOCKED = 286, // LOCKED
         TOK_LOGGED = 287, // LOGGED
         TOK_MAPPING = 288, // MAPPING
         TOK_MATCH = 289, // MATCH
         TOK_MATCHED = 290, // MATCHED
         TOK_MATERIALIZED = 291, // MATERIALIZED
         TOK_MAXVALUE = 292, // MAXVALUE
         TOK_MERGE = 293, // MERGE
         TOK_MERGE_ACTION = 294, // MERGE_ACTION
         TOK_METHOD = 295, // METHOD
         TOK_MINUTE_P = 296, // MINUTE_P
         TOK_MINVALUE = 297, // MINVALUE
         TOK_MODE = 298, // MODE
         TOK_MONTH_P = 299, // MONTH_P
         TOK_MOVE = 300, // MOVE
         TOK_NAME_P = 301, // NAME_P
         TOK_NAMES = 302, // NAMES
         TOK_NATIONAL = 303, // NATIONAL
         TOK_NATURAL = 304, // NATURAL
         TOK_NCHAR = 305, // NCHAR
         TOK_NESTED = 306, // NESTED
         TOK_NEW = 307, // NEW
         TOK_NEXT = 308, // NEXT
         TOK_NFC = 309, // NFC
         TOK_NFD = 310, // NFD
         TOK_NFKC = 311, // NFKC
         TOK_NFKD = 312, // NFKD
         TOK_NO = 313, // NO
         TOK_NONE = 314, // NONE
         TOK_NORMALIZE = 315, // NORMALIZE
         TOK_NORMALIZED = 316, // NORMALIZED
         TOK_NOT = 317, // NOT
         TOK_NOTHING = 318, // NOTHING
         TOK_NOTIFY = 319, // NOTIFY
         TOK_NOTNULL = 320, // NOTNULL
         TOK_NOWAIT = 321, // NOWAIT
         TOK_NULL_P = 322, // NULL_P
         TOK_NULLIF = 323, // NULLIF
         TOK_NULLS_P = 324, // NULLS_P
         TOK_NUMERIC = 325, // NUMERIC
         TOK_OBJECT_P = 326, // OBJECT_P
         TOK_OBJECTS_P = 327, // OBJECTS_P
         TOK_OF = 328, // OF
         TOK_OFF = 329, // OFF
         TOK_OFFSET = 330, // OFFSET
         TOK_OIDS = 331, // OIDS
         TOK_OLD = 332, // OLD
         TOK_OMIT = 333, // OMIT
         TOK_ON = 334, // ON
         TOK_ONLY = 335, // ONLY
         TOK_OPERATOR = 336, // OPERATOR
         TOK_OPTION = 337, // OPTION
         TOK_OPTIONS = 338, // OPTIONS
         TOK_OR = 339, // OR
         TOK_ORDER = 340, // ORDER
         TOK_ORDINALITY = 341, // ORDINALITY
         TOK_OTHERS = 342, // OTHERS
         TOK_OUT_P = 343, // OUT_P
         TOK_OUTER_P = 344, // OUTER_P
         TOK_OVER = 345, // OVER
         TOK_OVERLAPS = 346, // OVERLAPS
         TOK_OVERLAY = 347, // OVERLAY
         TOK_OVERRIDING = 348, // OVERRIDING
         TOK_OWNED = 349, // OWNED
         TOK_OWNER = 350, // OWNER
         TOK_PARALLEL = 351, // PARALLEL
         TOK_PARAMETER = 352, // PARAMETER
         TOK_PARSER = 353, // PARSER
         TOK_PARTIAL = 354, // PARTIAL
         TOK_PARTITION = 355, // PARTITION
         TOK_PASSING = 356, // PASSING
         TOK_PASSWORD = 357, // PASSWORD
         TOK_PATH = 358, // PATH
         TOK_PERIOD = 359, // PERIOD
         TOK_PLACING = 360, // PLACING
         TOK_PLAN = 361, // PLAN
         TOK_PLANS = 362, // PLANS
         TOK_POLICY = 363, // POLICY
         TOK_POSITION = 364, // POSITION
         TOK_PRECEDING = 365, // PRECEDING
         TOK_PRECISION = 366, // PRECISION
         TOK_PRESERVE = 367, // PRESERVE
         TOK_PREPARE = 368, // PREPARE
         TOK_PREPARED = 369, // PREPARED
         TOK_PRIMARY = 370, // PRIMARY
         TOK_PRIOR = 371, // PRIOR
         TOK_PRIVILEGES = 372, // PRIVILEGES
         TOK_PROCEDURAL = 373, // PROCEDURAL
         TOK_PROCEDURE = 374, // PROCEDURE
         TOK_PROCEDURES = 375, // PROCEDURES
         TOK_PROGRAM = 376, // PROGRAM
         TOK_PUBLICATION = 377, // PUBLICATION
         TOK_QUOTES = 378, // QUOTES
         TOK_EXTEND = 379, // EXTEND
         TOK_RANGE = 380, // RANGE
         TOK_READ = 381, // READ
         TOK_REAL = 382, // REAL
         TOK_REASSIGN = 383, // REASSIGN
         TOK_RECURSIVE = 384, // RECURSIVE
         TOK_REF_P = 385, // REF_P
         TOK_REFERENCES = 386, // REFERENCES
         TOK_REFERENCING = 387, // REFERENCING
         TOK_REFRESH = 388, // REFRESH
         TOK_REINDEX = 389, // REINDEX
         TOK_RELATIVE_P = 390, // RELATIVE_P
         TOK_RELEASE = 391, // RELEASE
         TOK_RENAME = 392, // RENAME
         TOK_REPEATABLE = 393, // REPEATABLE
         TOK_REPLACE = 394, // REPLACE
         TOK_REPLICA = 395, // REPLICA
         TOK_RESET = 396, // RESET
         TOK_RESTART = 397, // RESTART
         TOK_RESTRICT = 398, // RESTRICT
         TOK_RETURN = 399, // RETURN
         TOK_RETURNING = 400, // RETURNING
         TOK_RETURNS = 401, // RETURNS
         TOK_REVOKE = 402, // REVOKE
         TOK_RIGHT = 403, // RIGHT
         TOK_ROLE = 404, // ROLE
         TOK_ROLLBACK = 405, // ROLLBACK
         TOK_ROLLUP = 406, // ROLLUP
         TOK_ROUTINE = 407, // ROUTINE
         TOK_ROUTINES = 408, // ROUTINES
         TOK_ROW = 409, // ROW
         TOK_ROWS = 410, // ROWS
         TOK_RULE = 411, // RULE
         TOK_SAVEPOINT = 412, // SAVEPOINT
         TOK_SCALAR = 413, // SCALAR
         TOK_SCHEMA = 414, // SCHEMA
         TOK_SCHEMAS = 415, // SCHEMAS
         TOK_SCROLL = 416, // SCROLL
         TOK_SEARCH = 417, // SEARCH
         TOK_SECOND_P = 418, // SECOND_P
         TOK_SECURITY = 419, // SECURITY
         TOK_SELECT = 420, // SELECT
         TOK_SEQUENCE = 421, // SEQUENCE
         TOK_SEQUENCES = 422, // SEQUENCES
         TOK_SERIALIZABLE = 423, // SERIALIZABLE
         TOK_SERVER = 424, // SERVER
         TOK_SESSION = 425, // SESSION
         TOK_SESSION_USER = 426, // SESSION_USER
         TOK_SET = 427, // SET
         TOK_SETS = 428, // SETS
         TOK_SETOF = 429, // SETOF
         TOK_SHARE = 430, // SHARE
         TOK_SHOW = 431, // SHOW
         TOK_SIMILAR = 432, // SIMILAR
         TOK_SIMPLE = 433, // SIMPLE
         TOK_SKIP = 434, // SKIP
         TOK_SMALLINT = 435, // SMALLINT
         TOK_SNAPSHOT = 436, // SNAPSHOT
         TOK_SOME = 437, // SOME
         TOK_SOURCE = 438, // SOURCE
         TOK_SQL_P = 439, // SQL_P
         TOK_STABLE = 440, // STABLE
         TOK_STANDALONE_P = 441, // STANDALONE_P
         TOK_START = 442, // START
         TOK_STATEMENT = 443, // STATEMENT
         TOK_STATISTICS = 444, // STATISTICS
         TOK_STDIN = 445, // STDIN
         TOK_STDOUT = 446, // STDOUT
         TOK_STORAGE = 447, // STORAGE
         TOK_STORED = 448, // STORED
         TOK_STRICT_P = 449, // STRICT_P
         TOK_STRING_P = 450, // STRING_P
         TOK_STRIP_P = 451, // STRIP_P
         TOK_SUBSCRIPTION = 452, // SUBSCRIPTION
         TOK_SUBSTRING = 453, // SUBSTRING
         TOK_SUPPORT = 454, // SUPPORT
         TOK_SYMMETRIC = 455, // SYMMETRIC
         TOK_SYSID = 456, // SYSID
         TOK_SYSTEM_P = 457, // SYSTEM_P
         TOK_SYSTEM_USER = 458, // SYSTEM_USER
         TOK_TABLE = 459, // TABLE
         TOK_TABLES = 460, // TABLES
         TOK_TABLESAMPLE = 461, // TABLESAMPLE
         TOK_TABLESPACE = 462, // TABLESPACE
         TOK_TARGET = 463, // TARGET
         TOK_TEMP = 464, // TEMP
         TOK_TEMPLATE = 465, // TEMPLATE
         TOK_TEMPORARY = 466, // TEMPORARY
         TOK_TEXT_P = 467, // TEXT_P
         TOK_THEN = 468, // THEN
         TOK_TIES = 469, // TIES
         TOK_TIME = 470, // TIME
         TOK_TIMESTAMP = 471, // TIMESTAMP
         TOK_TO = 472, // TO
         TOK_TRAILING = 473, // TRAILING
         TOK_TRANSACTION = 474, // TRANSACTION
         TOK_TRANSFORM = 475, // TRANSFORM
         TOK_TREAT = 476, // TREAT
         TOK_TRIGGER = 477, // TRIGGER
         TOK_TRIM = 478, // TRIM
         TOK_TRUE_P = 479, // TRUE_P
         TOK_TRUNCATE = 480, // TRUNCATE
         TOK_TRUSTED = 481, // TRUSTED
         TOK_TYPE_P = 482, // TYPE_P
         TOK_TYPES_P = 483, // TYPES_P
         TOK_UESCAPE = 484, // UESCAPE
         TOK_UNBOUNDED = 485, // UNBOUNDED
         TOK_UNCONDITIONAL = 486, // UNCONDITIONAL
         TOK_UNCOMMITTED = 487, // UNCOMMITTED
         TOK_UNENCRYPTED = 488, // UNENCRYPTED
         TOK_UNION = 489, // UNION
         TOK_UNIQUE = 490, // UNIQUE
         TOK_UNKNOWN = 491, // UNKNOWN
         TOK_UNLISTEN = 492, // UNLISTEN
         TOK_UNLOGGED = 493, // UNLOGGED
         TOK_UNTIL = 494, // UNTIL
         TOK_UPDATE = 495, // UPDATE
         TOK_USER = 496, // USER
         TOK_USING = 497, // USING
         TOK_VACUUM = 498, // VACUUM
         TOK_VALID = 499, // VALID
         TOK_VALIDATE = 500, // VALIDATE
         TOK_VALIDATOR = 501, // VALIDATOR
         TOK_VALUE_P = 502, // VALUE_P
         TOK_VALUES = 503, // VALUES
         TOK_VARCHAR = 504, // VARCHAR
         TOK_VARIADIC = 505, // VARIADIC
         TOK_VARYING = 506, // VARYING
         TOK_VERBOSE = 507, // VERBOSE
         TOK_VERSION_P = 508, // VERSION_P
         TOK_VIEW = 509, // VIEW
         TOK_VIEWS = 510, // VIEWS
         TOK_VIRTUAL = 511, // VIRTUAL
         TOK_VOLATILE = 512, // VOLATILE
         TOK_WHEN = 513, // WHEN
         TOK_WHERE = 514, // WHERE
         TOK_WHITESPACE_P = 515, // WHITESPACE_P
         TOK_WINDOW = 516, // WINDOW
         TOK_WITH = 517, // WITH
         TOK_WITHIN = 518, // WITHIN
         TOK_WITHOUT = 519, // WITHOUT
         TOK_WORK = 520, // WORK
         TOK_WRAPPER = 521, // WRAPPER
         TOK_WRITE = 522, // WRITE
         TOK_XML_P = 523, // XML_P
         TOK_XMLATTRIBUTES = 524, // XMLATTRIBUTES
         TOK_XMLCONCAT = 525, // XMLCONCAT
         TOK_XMLELEMENT = 526, // XMLELEMENT
         TOK_XMLEXISTS = 527, // XMLEXISTS
         TOK_XMLFOREST = 528, // XMLFOREST
         TOK_XMLNAMESPACES = 529, // XMLNAMESPACES
         TOK_XMLPARSE = 530, // XMLPARSE
         TOK_XMLPI = 531, // XMLPI
         TOK_XMLROOT = 532, // XMLROOT
         TOK_XMLSERIALIZE = 533, // XMLSERIALIZE
         TOK_XMLTABLE = 534, // XMLTABLE
         TOK_YEAR_P = 535, // YEAR_P
         TOK_YES_P = 536, // YES_P
         TOK_ZONE = 537, // ZONE
         TOK_FORMAT_LA = 538, // FORMAT_LA
         TOK_NOT_LA = 539, // NOT_LA
         TOK_NULLS_LA = 540, // NULLS_LA
         TOK_WITH_LA = 541, // WITH_LA
         TOK_WITHOUT_LA = 542, // WITHOUT_LA
         TOK_AND_PRIORITY = 543, // AND_PRIORITY
         TOK_OR_PRIORITY = 544, // OR_PRIORITY
         TOK_QUAL_OP = 545, // QUAL_OP
         TOK_UMINUS = 546, // UMINUS
         TOK_ROLLUP_PRIORITY = 547 // ROLLUP_PRIORITY
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
   };

   /// Token kind, as returned by yylex.
   typedef token::token_kind_type token_kind_type;

   /// Backward compatibility alias (Bison 3.6).
   typedef token_kind_type token_type;

   /// Symbol kinds.
   struct symbol_kind {
      enum symbol_kind_type {
         YYNTOKENS = 548, ///< Number of tokens.
         S_YYEMPTY = -2,
         S_YYEOF = 0, // "end of file"
         S_YYerror = 1, // error
         S_YYUNDEF = 2, // "invalid token"
         S_ICONST = 3, // ICONST
         S_INTEGER_VALUE = 4, // "integer_value"
         S_FCONST = 5, // FCONST
         S_IDENT = 6, // IDENT
         S_IDENTIFIER = 7, // "identifier"
         S_STRING_VALUE = 8, // STRING_VALUE
         S_BIT_VALUE = 9, // "bit_string"
         S_HEX_VALUE = 10, // "hex_string"
         S_NATIONAL_VALUE = 11, // "nat_string"
         S_LP = 12, // "("
         S_RP = 13, // ")"
         S_LB = 14, // "["
         S_RB = 15, // "]"
         S_DOT = 16, // "."
         S_PERCENT = 17, // "%"
         S_COMMA = 18, // ","
         S_SEMICOLON = 19, // ";"
         S_PLUS = 20, // "+"
         S_SLASH = 21, // "/"
         S_STAR = 22, // "*"
         S_MINUS = 23, // "-"
         S_EQUAL = 24, // "="
         S_NOT_EQUAL = 25, // "<>"
         S_LESS_EQUAL = 26, // "<="
         S_LESS = 27, // "<"
         S_GREATER_EQUAL = 28, // ">="
         S_GREATER = 29, // ">"
         S_HAT = 30, // "^"
         S_QUOTE = 31, // "'"
         S_PIPE = 32, // "|>"
         S_UIDENT = 33, // UIDENT
         S_SCONST = 34, // SCONST
         S_USCONST = 35, // USCONST
         S_BCONST = 36, // BCONST
         S_XCONST = 37, // XCONST
         S_Op = 38, // Op
         S_PARAM = 39, // PARAM
         S_TYPECAST = 40, // TYPECAST
         S_DOT_DOT = 41, // DOT_DOT
         S_COLON_EQUALS = 42, // COLON_EQUALS
         S_ABORT_P = 43, // ABORT_P
         S_ABSENT = 44, // ABSENT
         S_ABSOLUTE_P = 45, // ABSOLUTE_P
         S_ACCESS = 46, // ACCESS
         S_ACTION = 47, // ACTION
         S_ADD_P = 48, // ADD_P
         S_ADMIN = 49, // ADMIN
         S_AFTER = 50, // AFTER
         S_AGGREGATE = 51, // AGGREGATE
         S_ALL = 52, // ALL
         S_ALSO = 53, // ALSO
         S_ALTER = 54, // ALTER
         S_ALWAYS = 55, // ALWAYS
         S_ANALYSE = 56, // ANALYSE
         S_ANALYZE = 57, // ANALYZE
         S_AND = 58, // AND
         S_ANY = 59, // ANY
         S_ARRAY = 60, // ARRAY
         S_AS = 61, // AS
         S_ASC = 62, // ASC
         S_ASENSITIVE = 63, // ASENSITIVE
         S_ASSERTION = 64, // ASSERTION
         S_ASSIGNMENT = 65, // ASSIGNMENT
         S_ASYMMETRIC = 66, // ASYMMETRIC
         S_ATOMIC = 67, // ATOMIC
         S_AT = 68, // AT
         S_ATTACH = 69, // ATTACH
         S_ATTRIBUTE = 70, // ATTRIBUTE
         S_AUTHORIZATION = 71, // AUTHORIZATION
         S_BACKWARD = 72, // BACKWARD
         S_BEFORE = 73, // BEFORE
         S_BEGIN_P = 74, // BEGIN_P
         S_BETWEEN = 75, // BETWEEN
         S_BIGINT = 76, // BIGINT
         S_BINARY = 77, // BINARY
         S_BIT = 78, // BIT
         S_BOOLEAN_P = 79, // BOOLEAN_P
         S_BOTH = 80, // BOTH
         S_BREADTH = 81, // BREADTH
         S_BY = 82, // BY
         S_CACHE = 83, // CACHE
         S_CALL = 84, // CALL
         S_CALLED = 85, // CALLED
         S_CASCADE = 86, // CASCADE
         S_CASCADED = 87, // CASCADED
         S_CASE = 88, // CASE
         S_CAST = 89, // CAST
         S_CATALOG_P = 90, // CATALOG_P
         S_CHAIN = 91, // CHAIN
         S_CHAR_P = 92, // CHAR_P
         S_CHARACTER = 93, // CHARACTER
         S_CHARACTERISTICS = 94, // CHARACTERISTICS
         S_CHECK = 95, // CHECK
         S_CHECKPOINT = 96, // CHECKPOINT
         S_CLASS = 97, // CLASS
         S_CLOSE = 98, // CLOSE
         S_CLUSTER = 99, // CLUSTER
         S_COALESCE = 100, // COALESCE
         S_COLLATE = 101, // COLLATE
         S_COLLATION = 102, // COLLATION
         S_COLUMN = 103, // COLUMN
         S_COLUMNS = 104, // COLUMNS
         S_COMMENT = 105, // COMMENT
         S_COMMENTS = 106, // COMMENTS
         S_COMMIT = 107, // COMMIT
         S_COMMITTED = 108, // COMMITTED
         S_COMPRESSION = 109, // COMPRESSION
         S_CONCURRENTLY = 110, // CONCURRENTLY
         S_CONDITIONAL = 111, // CONDITIONAL
         S_CONFIGURATION = 112, // CONFIGURATION
         S_CONFLICT = 113, // CONFLICT
         S_CONNECTION = 114, // CONNECTION
         S_CONSTRAINT = 115, // CONSTRAINT
         S_CONSTRAINTS = 116, // CONSTRAINTS
         S_CONTENT_P = 117, // CONTENT_P
         S_CONTINUE_P = 118, // CONTINUE_P
         S_CONVERSION_P = 119, // CONVERSION_P
         S_COPY = 120, // COPY
         S_COST = 121, // COST
         S_CREATE = 122, // CREATE
         S_CROSS = 123, // CROSS
         S_CSV = 124, // CSV
         S_CUBE = 125, // CUBE
         S_CURRENT_P = 126, // CURRENT_P
         S_CURRENT_CATALOG = 127, // CURRENT_CATALOG
         S_CURRENT_DATE = 128, // CURRENT_DATE
         S_CURRENT_ROLE = 129, // CURRENT_ROLE
         S_CURRENT_SCHEMA = 130, // CURRENT_SCHEMA
         S_CURRENT_TIME = 131, // CURRENT_TIME
         S_CURRENT_TIMESTAMP = 132, // CURRENT_TIMESTAMP
         S_CURRENT_USER = 133, // CURRENT_USER
         S_CURSOR = 134, // CURSOR
         S_CYCLE = 135, // CYCLE
         S_DATA_P = 136, // DATA_P
         S_DATABASE = 137, // DATABASE
         S_DAY_P = 138, // DAY_P
         S_DEALLOCATE = 139, // DEALLOCATE
         S_DEC = 140, // DEC
         S_DECIMAL_P = 141, // DECIMAL_P
         S_DECLARE = 142, // DECLARE
         S_DEFAULT = 143, // DEFAULT
         S_DEFAULTS = 144, // DEFAULTS
         S_DATE_P = 145, // DATE_P
         S_DEFERRABLE = 146, // DEFERRABLE
         S_DEFERRED = 147, // DEFERRED
         S_DEFINER = 148, // DEFINER
         S_DELETE_P = 149, // DELETE_P
         S_DELIMITER = 150, // DELIMITER
         S_DELIMITERS = 151, // DELIMITERS
         S_DEPENDS = 152, // DEPENDS
         S_DEPTH = 153, // DEPTH
         S_DESC = 154, // DESC
         S_DETACH = 155, // DETACH
         S_DICTIONARY = 156, // DICTIONARY
         S_DISABLE_P = 157, // DISABLE_P
         S_DISCARD = 158, // DISCARD
         S_DISTINCT = 159, // DISTINCT
         S_DO = 160, // DO
         S_DOCUMENT_P = 161, // DOCUMENT_P
         S_DOMAIN_P = 162, // DOMAIN_P
         S_DOUBLE_P = 163, // DOUBLE_P
         S_DROP = 164, // DROP
         S_EACH = 165, // EACH
         S_ELSE = 166, // ELSE
         S_EMPTY_P = 167, // EMPTY_P
         S_ENABLE_P = 168, // ENABLE_P
         S_ENCODING = 169, // ENCODING
         S_ENCRYPTED = 170, // ENCRYPTED
         S_END_P = 171, // END_P
         S_ENFORCED = 172, // ENFORCED
         S_ENUM_P = 173, // ENUM_P
         S_ERROR_P = 174, // ERROR_P
         S_ESCAPE = 175, // ESCAPE
         S_EVENT = 176, // EVENT
         S_EXCEPT = 177, // EXCEPT
         S_EXCLUDE = 178, // EXCLUDE
         S_EXCLUDING = 179, // EXCLUDING
         S_EXCLUSIVE = 180, // EXCLUSIVE
         S_EXECUTE = 181, // EXECUTE
         S_EXISTS = 182, // EXISTS
         S_EXPLAIN = 183, // EXPLAIN
         S_EXPRESSION = 184, // EXPRESSION
         S_EXTENSION = 185, // EXTENSION
         S_EXTERNAL = 186, // EXTERNAL
         S_EXTRACT = 187, // EXTRACT
         S_FALSE_P = 188, // FALSE_P
         S_FAMILY = 189, // FAMILY
         S_FETCH = 190, // FETCH
         S_FILTER = 191, // FILTER
         S_FINALIZE = 192, // FINALIZE
         S_FIRST_P = 193, // FIRST_P
         S_FLOAT_P = 194, // FLOAT_P
         S_FOLLOWING = 195, // FOLLOWING
         S_FOR = 196, // FOR
         S_FORCE = 197, // FORCE
         S_FOREIGN = 198, // FOREIGN
         S_FORMAT = 199, // FORMAT
         S_FORWARD = 200, // FORWARD
         S_FREEZE = 201, // FREEZE
         S_FROM = 202, // FROM
         S_FULL = 203, // FULL
         S_FUNCTION = 204, // FUNCTION
         S_FUNCTIONS = 205, // FUNCTIONS
         S_GENERATED = 206, // GENERATED
         S_GLOBAL = 207, // GLOBAL
         S_GRANT = 208, // GRANT
         S_GRANTED = 209, // GRANTED
         S_GREATEST = 210, // GREATEST
         S_GROUP_P = 211, // GROUP_P
         S_GROUPING = 212, // GROUPING
         S_GROUPS = 213, // GROUPS
         S_HANDLER = 214, // HANDLER
         S_HAVING = 215, // HAVING
         S_HEADER_P = 216, // HEADER_P
         S_HOLD = 217, // HOLD
         S_HOUR_P = 218, // HOUR_P
         S_IDENTITY_P = 219, // IDENTITY_P
         S_IF_P = 220, // IF_P
         S_ILIKE = 221, // ILIKE
         S_IMMEDIATE = 222, // IMMEDIATE
         S_IMMUTABLE = 223, // IMMUTABLE
         S_IMPLICIT_P = 224, // IMPLICIT_P
         S_IMPORT_P = 225, // IMPORT_P
         S_IN_P = 226, // IN_P
         S_INCLUDE = 227, // INCLUDE
         S_INCLUDING = 228, // INCLUDING
         S_INCREMENT = 229, // INCREMENT
         S_INDENT = 230, // INDENT
         S_INDEX = 231, // INDEX
         S_INDEXES = 232, // INDEXES
         S_INHERIT = 233, // INHERIT
         S_INHERITS = 234, // INHERITS
         S_INITIALLY = 235, // INITIALLY
         S_INLINE_P = 236, // INLINE_P
         S_INNER_P = 237, // INNER_P
         S_INOUT = 238, // INOUT
         S_INPUT_P = 239, // INPUT_P
         S_INSENSITIVE = 240, // INSENSITIVE
         S_INSERT = 241, // INSERT
         S_INSTEAD = 242, // INSTEAD
         S_INT_P = 243, // INT_P
         S_INTEGER = 244, // INTEGER
         S_INTERSECT = 245, // INTERSECT
         S_INTERVAL = 246, // INTERVAL
         S_INTO = 247, // INTO
         S_INVOKER = 248, // INVOKER
         S_IS = 249, // IS
         S_ISNULL = 250, // ISNULL
         S_ISOLATION = 251, // ISOLATION
         S_JOIN = 252, // JOIN
         S_JSON = 253, // JSON
         S_JSON_ARRAY = 254, // JSON_ARRAY
         S_JSON_ARRAYAGG = 255, // JSON_ARRAYAGG
         S_JSON_EXISTS = 256, // JSON_EXISTS
         S_JSON_OBJECT = 257, // JSON_OBJECT
         S_JSON_OBJECTAGG = 258, // JSON_OBJECTAGG
         S_JSON_QUERY = 259, // JSON_QUERY
         S_JSON_SCALAR = 260, // JSON_SCALAR
         S_JSON_SERIALIZE = 261, // JSON_SERIALIZE
         S_JSON_TABLE = 262, // JSON_TABLE
         S_JSON_VALUE = 263, // JSON_VALUE
         S_KEEP = 264, // KEEP
         S_KEY = 265, // KEY
         S_KEYS = 266, // KEYS
         S_LABEL = 267, // LABEL
         S_LANGUAGE = 268, // LANGUAGE
         S_LARGE_P = 269, // LARGE_P
         S_LAST_P = 270, // LAST_P
         S_LATERAL_P = 271, // LATERAL_P
         S_LEADING = 272, // LEADING
         S_LEAKPROOF = 273, // LEAKPROOF
         S_LEAST = 274, // LEAST
         S_LEFT = 275, // LEFT
         S_LEVEL = 276, // LEVEL
         S_LIKE = 277, // LIKE
         S_LIMIT = 278, // LIMIT
         S_LISTEN = 279, // LISTEN
         S_LOAD = 280, // LOAD
         S_LOCAL = 281, // LOCAL
         S_LOCALTIME = 282, // LOCALTIME
         S_LOCALTIMESTAMP = 283, // LOCALTIMESTAMP
         S_LOCATION = 284, // LOCATION
         S_LOCK_P = 285, // LOCK_P
         S_LOCKED = 286, // LOCKED
         S_LOGGED = 287, // LOGGED
         S_MAPPING = 288, // MAPPING
         S_MATCH = 289, // MATCH
         S_MATCHED = 290, // MATCHED
         S_MATERIALIZED = 291, // MATERIALIZED
         S_MAXVALUE = 292, // MAXVALUE
         S_MERGE = 293, // MERGE
         S_MERGE_ACTION = 294, // MERGE_ACTION
         S_METHOD = 295, // METHOD
         S_MINUTE_P = 296, // MINUTE_P
         S_MINVALUE = 297, // MINVALUE
         S_MODE = 298, // MODE
         S_MONTH_P = 299, // MONTH_P
         S_MOVE = 300, // MOVE
         S_NAME_P = 301, // NAME_P
         S_NAMES = 302, // NAMES
         S_NATIONAL = 303, // NATIONAL
         S_NATURAL = 304, // NATURAL
         S_NCHAR = 305, // NCHAR
         S_NESTED = 306, // NESTED
         S_NEW = 307, // NEW
         S_NEXT = 308, // NEXT
         S_NFC = 309, // NFC
         S_NFD = 310, // NFD
         S_NFKC = 311, // NFKC
         S_NFKD = 312, // NFKD
         S_NO = 313, // NO
         S_NONE = 314, // NONE
         S_NORMALIZE = 315, // NORMALIZE
         S_NORMALIZED = 316, // NORMALIZED
         S_NOT = 317, // NOT
         S_NOTHING = 318, // NOTHING
         S_NOTIFY = 319, // NOTIFY
         S_NOTNULL = 320, // NOTNULL
         S_NOWAIT = 321, // NOWAIT
         S_NULL_P = 322, // NULL_P
         S_NULLIF = 323, // NULLIF
         S_NULLS_P = 324, // NULLS_P
         S_NUMERIC = 325, // NUMERIC
         S_OBJECT_P = 326, // OBJECT_P
         S_OBJECTS_P = 327, // OBJECTS_P
         S_OF = 328, // OF
         S_OFF = 329, // OFF
         S_OFFSET = 330, // OFFSET
         S_OIDS = 331, // OIDS
         S_OLD = 332, // OLD
         S_OMIT = 333, // OMIT
         S_ON = 334, // ON
         S_ONLY = 335, // ONLY
         S_OPERATOR = 336, // OPERATOR
         S_OPTION = 337, // OPTION
         S_OPTIONS = 338, // OPTIONS
         S_OR = 339, // OR
         S_ORDER = 340, // ORDER
         S_ORDINALITY = 341, // ORDINALITY
         S_OTHERS = 342, // OTHERS
         S_OUT_P = 343, // OUT_P
         S_OUTER_P = 344, // OUTER_P
         S_OVER = 345, // OVER
         S_OVERLAPS = 346, // OVERLAPS
         S_OVERLAY = 347, // OVERLAY
         S_OVERRIDING = 348, // OVERRIDING
         S_OWNED = 349, // OWNED
         S_OWNER = 350, // OWNER
         S_PARALLEL = 351, // PARALLEL
         S_PARAMETER = 352, // PARAMETER
         S_PARSER = 353, // PARSER
         S_PARTIAL = 354, // PARTIAL
         S_PARTITION = 355, // PARTITION
         S_PASSING = 356, // PASSING
         S_PASSWORD = 357, // PASSWORD
         S_PATH = 358, // PATH
         S_PERIOD = 359, // PERIOD
         S_PLACING = 360, // PLACING
         S_PLAN = 361, // PLAN
         S_PLANS = 362, // PLANS
         S_POLICY = 363, // POLICY
         S_POSITION = 364, // POSITION
         S_PRECEDING = 365, // PRECEDING
         S_PRECISION = 366, // PRECISION
         S_PRESERVE = 367, // PRESERVE
         S_PREPARE = 368, // PREPARE
         S_PREPARED = 369, // PREPARED
         S_PRIMARY = 370, // PRIMARY
         S_PRIOR = 371, // PRIOR
         S_PRIVILEGES = 372, // PRIVILEGES
         S_PROCEDURAL = 373, // PROCEDURAL
         S_PROCEDURE = 374, // PROCEDURE
         S_PROCEDURES = 375, // PROCEDURES
         S_PROGRAM = 376, // PROGRAM
         S_PUBLICATION = 377, // PUBLICATION
         S_QUOTES = 378, // QUOTES
         S_EXTEND = 379, // EXTEND
         S_RANGE = 380, // RANGE
         S_READ = 381, // READ
         S_REAL = 382, // REAL
         S_REASSIGN = 383, // REASSIGN
         S_RECURSIVE = 384, // RECURSIVE
         S_REF_P = 385, // REF_P
         S_REFERENCES = 386, // REFERENCES
         S_REFERENCING = 387, // REFERENCING
         S_REFRESH = 388, // REFRESH
         S_REINDEX = 389, // REINDEX
         S_RELATIVE_P = 390, // RELATIVE_P
         S_RELEASE = 391, // RELEASE
         S_RENAME = 392, // RENAME
         S_REPEATABLE = 393, // REPEATABLE
         S_REPLACE = 394, // REPLACE
         S_REPLICA = 395, // REPLICA
         S_RESET = 396, // RESET
         S_RESTART = 397, // RESTART
         S_RESTRICT = 398, // RESTRICT
         S_RETURN = 399, // RETURN
         S_RETURNING = 400, // RETURNING
         S_RETURNS = 401, // RETURNS
         S_REVOKE = 402, // REVOKE
         S_RIGHT = 403, // RIGHT
         S_ROLE = 404, // ROLE
         S_ROLLBACK = 405, // ROLLBACK
         S_ROLLUP = 406, // ROLLUP
         S_ROUTINE = 407, // ROUTINE
         S_ROUTINES = 408, // ROUTINES
         S_ROW = 409, // ROW
         S_ROWS = 410, // ROWS
         S_RULE = 411, // RULE
         S_SAVEPOINT = 412, // SAVEPOINT
         S_SCALAR = 413, // SCALAR
         S_SCHEMA = 414, // SCHEMA
         S_SCHEMAS = 415, // SCHEMAS
         S_SCROLL = 416, // SCROLL
         S_SEARCH = 417, // SEARCH
         S_SECOND_P = 418, // SECOND_P
         S_SECURITY = 419, // SECURITY
         S_SELECT = 420, // SELECT
         S_SEQUENCE = 421, // SEQUENCE
         S_SEQUENCES = 422, // SEQUENCES
         S_SERIALIZABLE = 423, // SERIALIZABLE
         S_SERVER = 424, // SERVER
         S_SESSION = 425, // SESSION
         S_SESSION_USER = 426, // SESSION_USER
         S_SET = 427, // SET
         S_SETS = 428, // SETS
         S_SETOF = 429, // SETOF
         S_SHARE = 430, // SHARE
         S_SHOW = 431, // SHOW
         S_SIMILAR = 432, // SIMILAR
         S_SIMPLE = 433, // SIMPLE
         S_SKIP = 434, // SKIP
         S_SMALLINT = 435, // SMALLINT
         S_SNAPSHOT = 436, // SNAPSHOT
         S_SOME = 437, // SOME
         S_SOURCE = 438, // SOURCE
         S_SQL_P = 439, // SQL_P
         S_STABLE = 440, // STABLE
         S_STANDALONE_P = 441, // STANDALONE_P
         S_START = 442, // START
         S_STATEMENT = 443, // STATEMENT
         S_STATISTICS = 444, // STATISTICS
         S_STDIN = 445, // STDIN
         S_STDOUT = 446, // STDOUT
         S_STORAGE = 447, // STORAGE
         S_STORED = 448, // STORED
         S_STRICT_P = 449, // STRICT_P
         S_STRING_P = 450, // STRING_P
         S_STRIP_P = 451, // STRIP_P
         S_SUBSCRIPTION = 452, // SUBSCRIPTION
         S_SUBSTRING = 453, // SUBSTRING
         S_SUPPORT = 454, // SUPPORT
         S_SYMMETRIC = 455, // SYMMETRIC
         S_SYSID = 456, // SYSID
         S_SYSTEM_P = 457, // SYSTEM_P
         S_SYSTEM_USER = 458, // SYSTEM_USER
         S_TABLE = 459, // TABLE
         S_TABLES = 460, // TABLES
         S_TABLESAMPLE = 461, // TABLESAMPLE
         S_TABLESPACE = 462, // TABLESPACE
         S_TARGET = 463, // TARGET
         S_TEMP = 464, // TEMP
         S_TEMPLATE = 465, // TEMPLATE
         S_TEMPORARY = 466, // TEMPORARY
         S_TEXT_P = 467, // TEXT_P
         S_THEN = 468, // THEN
         S_TIES = 469, // TIES
         S_TIME = 470, // TIME
         S_TIMESTAMP = 471, // TIMESTAMP
         S_TO = 472, // TO
         S_TRAILING = 473, // TRAILING
         S_TRANSACTION = 474, // TRANSACTION
         S_TRANSFORM = 475, // TRANSFORM
         S_TREAT = 476, // TREAT
         S_TRIGGER = 477, // TRIGGER
         S_TRIM = 478, // TRIM
         S_TRUE_P = 479, // TRUE_P
         S_TRUNCATE = 480, // TRUNCATE
         S_TRUSTED = 481, // TRUSTED
         S_TYPE_P = 482, // TYPE_P
         S_TYPES_P = 483, // TYPES_P
         S_UESCAPE = 484, // UESCAPE
         S_UNBOUNDED = 485, // UNBOUNDED
         S_UNCONDITIONAL = 486, // UNCONDITIONAL
         S_UNCOMMITTED = 487, // UNCOMMITTED
         S_UNENCRYPTED = 488, // UNENCRYPTED
         S_UNION = 489, // UNION
         S_UNIQUE = 490, // UNIQUE
         S_UNKNOWN = 491, // UNKNOWN
         S_UNLISTEN = 492, // UNLISTEN
         S_UNLOGGED = 493, // UNLOGGED
         S_UNTIL = 494, // UNTIL
         S_UPDATE = 495, // UPDATE
         S_USER = 496, // USER
         S_USING = 497, // USING
         S_VACUUM = 498, // VACUUM
         S_VALID = 499, // VALID
         S_VALIDATE = 500, // VALIDATE
         S_VALIDATOR = 501, // VALIDATOR
         S_VALUE_P = 502, // VALUE_P
         S_VALUES = 503, // VALUES
         S_VARCHAR = 504, // VARCHAR
         S_VARIADIC = 505, // VARIADIC
         S_VARYING = 506, // VARYING
         S_VERBOSE = 507, // VERBOSE
         S_VERSION_P = 508, // VERSION_P
         S_VIEW = 509, // VIEW
         S_VIEWS = 510, // VIEWS
         S_VIRTUAL = 511, // VIRTUAL
         S_VOLATILE = 512, // VOLATILE
         S_WHEN = 513, // WHEN
         S_WHERE = 514, // WHERE
         S_WHITESPACE_P = 515, // WHITESPACE_P
         S_WINDOW = 516, // WINDOW
         S_WITH = 517, // WITH
         S_WITHIN = 518, // WITHIN
         S_WITHOUT = 519, // WITHOUT
         S_WORK = 520, // WORK
         S_WRAPPER = 521, // WRAPPER
         S_WRITE = 522, // WRITE
         S_XML_P = 523, // XML_P
         S_XMLATTRIBUTES = 524, // XMLATTRIBUTES
         S_XMLCONCAT = 525, // XMLCONCAT
         S_XMLELEMENT = 526, // XMLELEMENT
         S_XMLEXISTS = 527, // XMLEXISTS
         S_XMLFOREST = 528, // XMLFOREST
         S_XMLNAMESPACES = 529, // XMLNAMESPACES
         S_XMLPARSE = 530, // XMLPARSE
         S_XMLPI = 531, // XMLPI
         S_XMLROOT = 532, // XMLROOT
         S_XMLSERIALIZE = 533, // XMLSERIALIZE
         S_XMLTABLE = 534, // XMLTABLE
         S_YEAR_P = 535, // YEAR_P
         S_YES_P = 536, // YES_P
         S_ZONE = 537, // ZONE
         S_FORMAT_LA = 538, // FORMAT_LA
         S_NOT_LA = 539, // NOT_LA
         S_NULLS_LA = 540, // NULLS_LA
         S_WITH_LA = 541, // WITH_LA
         S_WITHOUT_LA = 542, // WITHOUT_LA
         S_AND_PRIORITY = 543, // AND_PRIORITY
         S_OR_PRIORITY = 544, // OR_PRIORITY
         S_QUAL_OP = 545, // QUAL_OP
         S_UMINUS = 546, // UMINUS
         S_ROLLUP_PRIORITY = 547, // ROLLUP_PRIORITY
         S_YYACCEPT = 548, // $accept
         S_parse_toplevel = 549, // parse_toplevel
         S_stmtmulti = 550, // stmtmulti
         S_toplevel_stmt = 551, // toplevel_stmt
         S_stmt = 552, // stmt
         S_SelectStmt = 553, // SelectStmt
         S_PipeSQLStmt = 554, // PipeSQLStmt
         S_classic_select_and_pipe_sql_with_parens = 555, // classic_select_and_pipe_sql_with_parens
         S_select_with_parens = 556, // select_with_parens
         S_pipe_sql_with_parens = 557, // pipe_sql_with_parens
         S_select_no_parens = 558, // select_no_parens
         S_pipe_sql_no_parens = 559, // pipe_sql_no_parens
         S_pipe_start = 560, // pipe_start
         S_pipe_or_select_clause = 561, // pipe_or_select_clause
         S_select_clause = 562, // select_clause
         S_PreparableStmt = 563, // PreparableStmt
         S_CopyStmt = 564, // CopyStmt
         S_copy_from = 565, // copy_from
         S_copy_file_name = 566, // copy_file_name
         S_copy_options = 567, // copy_options
         S_copy_opt_list = 568, // copy_opt_list
         S_copy_opt_item = 569, // copy_opt_item
         S_copy_delimiter = 570, // copy_delimiter
         S_VariableSetStmt = 571, // VariableSetStmt
         S_set_rest = 572, // set_rest
         S_generic_set = 573, // generic_set
         S_set_rest_more = 574, // set_rest_more
         S_var_name = 575, // var_name
         S_var_list = 576, // var_list
         S_var_value = 577, // var_value
         S_opt_boolean_or_string = 578, // opt_boolean_or_string
         S_NonReservedWord_or_Sconst = 579, // NonReservedWord_or_Sconst
         S_NonReservedWord = 580, // NonReservedWord
         S_simple_select = 581, // simple_select
         S_with_clause = 582, // with_clause
         S_distinct_clause = 583, // distinct_clause
         S_cte_list = 584, // cte_list
         S_into_clause = 585, // into_clause
         S_window_clause = 586, // window_clause
         S_common_table_expr = 587, // common_table_expr
         S_opt_materialized = 588, // opt_materialized
         S_opt_from_clause = 589, // opt_from_clause
         S_from_clause = 590, // from_clause
         S_from_list = 591, // from_list
         S_table_ref = 592, // table_ref
         S_set_quantifier = 593, // set_quantifier
         S_group_clause = 594, // group_clause
         S_group_clause_with_alias = 595, // group_clause_with_alias
         S_group_by_list = 596, // group_by_list
         S_group_by_list_with_alias = 597, // group_by_list_with_alias
         S_rollup_clause = 598, // rollup_clause
         S_group_by_item = 599, // group_by_item
         S_group_by_item_with_alias = 600, // group_by_item_with_alias
         S_empty_grouping_set = 601, // empty_grouping_set
         S_having_clause = 602, // having_clause
         S_joined_table = 603, // joined_table
         S_alias_clause = 604, // alias_clause
         S_opt_nulls_order = 605, // opt_nulls_order
         S_opt_alias_clause = 606, // opt_alias_clause
         S_opt_asc_desc = 607, // opt_asc_desc
         S_join_type = 608, // join_type
         S_join_qual = 609, // join_qual
         S_relation_expr = 610, // relation_expr
         S_extended_relation_expr = 611, // extended_relation_expr
         S_opt_asymmetric = 612, // opt_asymmetric
         S_opt_all_clause = 613, // opt_all_clause
         S_opt_sort_clause = 614, // opt_sort_clause
         S_sort_clause = 615, // sort_clause
         S_sortby_list = 616, // sortby_list
         S_sortby = 617, // sortby
         S_select_limit = 618, // select_limit
         S_opt_select_limit = 619, // opt_select_limit
         S_limit_clause = 620, // limit_clause
         S_offset_clause = 621, // offset_clause
         S_select_limit_value = 622, // select_limit_value
         S_select_offset_value = 623, // select_offset_value
         S_values_clause = 624, // values_clause
         S_where_clause = 625, // where_clause
         S_a_expr = 626, // a_expr
         S_and_a_expr = 627, // and_a_expr
         S_or_a_expr = 628, // or_a_expr
         S_b_expr = 629, // b_expr
         S_c_expr = 630, // c_expr
         S_basicComparisonType = 631, // basicComparisonType
         S_in_expr = 632, // in_expr
         S_case_expr = 633, // case_expr
         S_when_clause_list = 634, // when_clause_list
         S_when_clause = 635, // when_clause
         S_case_default = 636, // case_default
         S_case_arg = 637, // case_arg
         S_columnref_list = 638, // columnref_list
         S_columnref = 639, // columnref
         S_func_application = 640, // func_application
         S_func_expr = 641, // func_expr
         S_window_func_expr = 642, // window_func_expr
         S_func_arg_list_opt = 643, // func_arg_list_opt
         S_func_arg_list = 644, // func_arg_list
         S_expr_list = 645, // expr_list
         S_expr_list_with_alias = 646, // expr_list_with_alias
         S_func_arg_expr = 647, // func_arg_expr
         S_func_expr_common_subexpr = 648, // func_expr_common_subexpr
         S_cast_expr = 649, // cast_expr
         S_extract_list = 650, // extract_list
         S_extract_arg = 651, // extract_arg
         S_substr_list = 652, // substr_list
         S_over_clause = 653, // over_clause
         S_window_specification = 654, // window_specification
         S_indirection = 655, // indirection
         S_indirection_el = 656, // indirection_el
         S_opt_target_list = 657, // opt_target_list
         S_target_list = 658, // target_list
         S_target_el = 659, // target_el
         S_opt_existing_window_name = 660, // opt_existing_window_name
         S_opt_partition_clause = 661, // opt_partition_clause
         S_opt_frame_clause = 662, // opt_frame_clause
         S_frame_extent = 663, // frame_extent
         S_frame_bound = 664, // frame_bound
         S_opt_window_exclusion_clause = 665, // opt_window_exclusion_clause
         S_any_operator = 666, // any_operator
         S_qual_Op = 667, // qual_Op
         S_qual_all_Op = 668, // qual_all_Op
         S_sub_type = 669, // sub_type
         S_all_Op = 670, // all_Op
         S_ColId = 671, // ColId
         S_type_function_name = 672, // type_function_name
         S_type_func_name_keyword = 673, // type_func_name_keyword
         S_ColLabel = 674, // ColLabel
         S_BareColLabel = 675, // BareColLabel
         S_reserved_keyword = 676, // reserved_keyword
         S_unreserved_keyword = 677, // unreserved_keyword
         S_col_name_keyword = 678, // col_name_keyword
         S_qualified_name = 679, // qualified_name
         S_opt_name_list = 680, // opt_name_list
         S_name_list = 681, // name_list
         S_name = 682, // name
         S_attr_name = 683, // attr_name
         S_func_name = 684, // func_name
         S_param_name = 685, // param_name
         S_CreateStmt = 686, // CreateStmt
         S_OptTemp = 687, // OptTemp
         S_OptTableElementList = 688, // OptTableElementList
         S_TableElementList = 689, // TableElementList
         S_TableElement = 690, // TableElement
         S_TableConstraint = 691, // TableConstraint
         S_columnElement = 692, // columnElement
         S_create_generic_options = 693, // create_generic_options
         S_ColQualList = 694, // ColQualList
         S_ColConstraint = 695, // ColConstraint
         S_ColConstraintElem = 696, // ColConstraintElem
         S_ConstraintElem = 697, // ConstraintElem
         S_opt_without_overlaps = 698, // opt_without_overlaps
         S_opt_column_compression = 699, // opt_column_compression
         S_opt_column_storage = 700, // opt_column_storage
         S_Type = 701, // Type
         S_SimpleType = 702, // SimpleType
         S_opt_type_modifiers = 703, // opt_type_modifiers
         S_type_modifiers = 704, // type_modifiers
         S_type_modifier = 705, // type_modifier
         S_ConstTypename = 706, // ConstTypename
         S_NumericOnly = 707, // NumericOnly
         S_Numeric_with_opt_lenghth = 708, // Numeric_with_opt_lenghth
         S_Numeric = 709, // Numeric
         S_Character = 710, // Character
         S_ConstCharacter = 711, // ConstCharacter
         S_CharacterWithLength = 712, // CharacterWithLength
         S_CharacterWithoutLength = 713, // CharacterWithoutLength
         S_character = 714, // character
         S_opt_varying = 715, // opt_varying
         S_ConstDatetime = 716, // ConstDatetime
         S_InsertStmt = 717, // InsertStmt
         S_insert_target = 718, // insert_target
         S_insert_rest = 719, // insert_rest
         S_insert_column_list = 720, // insert_column_list
         S_insert_column_item = 721, // insert_column_item
         S_AexprConst = 722, // AexprConst
         S_Iconst = 723, // Iconst
         S_SignedIconst = 724, // SignedIconst
         S_Fconst = 725, // Fconst
         S_Sconst = 726, // Sconst
         S_Bconst = 727, // Bconst
         S_ConstInterval = 728, // ConstInterval
         S_opt_interval = 729, // opt_interval
         S_Typename = 730, // Typename
         S_pipe_operator = 731, // pipe_operator
         S_agg_expr = 732, // agg_expr
         S_func_expr_list = 733, // func_expr_list
         S_set_list = 734 // set_list
      };
   };

   /// (Internal) symbol kind.
   typedef symbol_kind::symbol_kind_type symbol_kind_type;

   /// The number of tokens.
   static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

   /// A complete symbol.
   ///
   /// Expects its Base type to provide access to the symbol kind
   /// via kind ().
   ///
   /// Provide access to semantic value and location.
   template <typename Base>
   struct basic_symbol : Base {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol() YY_NOEXCEPT
         : value(),
           location() {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol(basic_symbol&& that)
         : Base(std::move(that)), value(), location(std::move(that.location)) {
         switch (this->kind()) {
            case symbol_kind::S_distinct_clause: // distinct_clause
            case symbol_kind::S_set_quantifier: // set_quantifier
            case symbol_kind::S_opt_asymmetric: // opt_asymmetric
            case symbol_kind::S_OptTemp: // OptTemp
            case symbol_kind::S_opt_varying: // opt_varying
               value.move<bool>(std::move(that.value));
               break;

            case symbol_kind::S_ICONST: // ICONST
               value.move<int>(std::move(that.value));
               break;

            case symbol_kind::S_when_clause: // when_clause
               value.move<lingodb::ast::CaseExpression::CaseCheck>(std::move(that.value));
               break;

            case symbol_kind::S_basicComparisonType: // basicComparisonType
               value.move<lingodb::ast::ExpressionType>(std::move(that.value));
               break;

            case symbol_kind::S_join_type: // join_type
               value.move<lingodb::ast::JoinType>(std::move(that.value));
               break;

            case symbol_kind::S_Type: // Type
            case symbol_kind::S_SimpleType: // SimpleType
            case symbol_kind::S_ConstTypename: // ConstTypename
            case symbol_kind::S_Numeric_with_opt_lenghth: // Numeric_with_opt_lenghth
            case symbol_kind::S_Numeric: // Numeric
            case symbol_kind::S_Character: // Character
            case symbol_kind::S_ConstCharacter: // ConstCharacter
            case symbol_kind::S_CharacterWithLength: // CharacterWithLength
            case symbol_kind::S_CharacterWithoutLength: // CharacterWithoutLength
            case symbol_kind::S_character: // character
            case symbol_kind::S_ConstDatetime: // ConstDatetime
            case symbol_kind::S_ConstInterval: // ConstInterval
            case symbol_kind::S_Typename: // Typename
               value.move<lingodb::ast::LogicalTypeWithMods>(std::move(that.value));
               break;

            case symbol_kind::S_opt_nulls_order: // opt_nulls_order
               value.move<lingodb::ast::OrderByNullType>(std::move(that.value));
               break;

            case symbol_kind::S_opt_asc_desc: // opt_asc_desc
               value.move<lingodb::ast::OrderType>(std::move(that.value));
               break;

            case symbol_kind::S_sub_type: // sub_type
               value.move<lingodb::ast::SubqueryType>(std::move(that.value));
               break;

            case symbol_kind::S_join_qual: // join_qual
               value.move<lingodb::ast::jointCondOrUsingCols>(std::move(that.value));
               break;

            case symbol_kind::S_opt_interval: // opt_interval
               value.move<std::optional<lingodb::ast::SQLAbstractLogicalType>>(std::move(that.value));
               break;

            case symbol_kind::S_opt_sort_clause: // opt_sort_clause
               value.move<std::optional<std::shared_ptr<lingodb::ast::OrderByModifier>>>(std::move(that.value));
               break;

            case symbol_kind::S_case_arg: // case_arg
               value.move<std::optional<std::shared_ptr<lingodb::ast::ParsedExpression>>>(std::move(that.value));
               break;

            case symbol_kind::S_opt_select_limit: // opt_select_limit
               value.move<std::optional<std::shared_ptr<lingodb::ast::ResultModifier>>>(std::move(that.value));
               break;

            case symbol_kind::S_copy_opt_item: // copy_opt_item
               value.move<std::pair<std::string, std::string>>(std::move(that.value));
               break;

            case symbol_kind::S_alias_clause: // alias_clause
            case symbol_kind::S_opt_alias_clause: // opt_alias_clause
               value.move<std::pair<std::string, std::vector<std::string>>>(std::move(that.value));
               break;

            case symbol_kind::S_extract_arg: // extract_arg
               value.move<std::shared_ptr<ast::ConstantExpression>>(std::move(that.value));
               break;

            case symbol_kind::S_CopyStmt: // CopyStmt
               value.move<std::shared_ptr<ast::CopyNode>>(std::move(that.value));
               break;

            case symbol_kind::S_VariableSetStmt: // VariableSetStmt
            case symbol_kind::S_set_rest: // set_rest
            case symbol_kind::S_generic_set: // generic_set
            case symbol_kind::S_set_rest_more: // set_rest_more
               value.move<std::shared_ptr<ast::SetVariableStatement>>(std::move(that.value));
               break;

            case symbol_kind::S_agg_expr: // agg_expr
               value.move<std::shared_ptr<lingodb::ast::AggregationNode>>(std::move(that.value));
               break;

            case symbol_kind::S_toplevel_stmt: // toplevel_stmt
            case symbol_kind::S_stmt: // stmt
               value.move<std::shared_ptr<lingodb::ast::AstNode>>(std::move(that.value));
               break;

            case symbol_kind::S_and_a_expr: // and_a_expr
            case symbol_kind::S_or_a_expr: // or_a_expr
               value.move<std::shared_ptr<lingodb::ast::ConjunctionExpression>>(std::move(that.value));
               break;

            case symbol_kind::S_ColConstraint: // ColConstraint
            case symbol_kind::S_ColConstraintElem: // ColConstraintElem
            case symbol_kind::S_ConstraintElem: // ConstraintElem
               value.move<std::shared_ptr<lingodb::ast::Constraint>>(std::move(that.value));
               break;

            case symbol_kind::S_CreateStmt: // CreateStmt
               value.move<std::shared_ptr<lingodb::ast::CreateNode>>(std::move(that.value));
               break;

            case symbol_kind::S_from_list: // from_list
               value.move<std::shared_ptr<lingodb::ast::CrossProductRef>>(std::move(that.value));
               break;

            case symbol_kind::S_values_clause: // values_clause
               value.move<std::shared_ptr<lingodb::ast::ExpressionListRef>>(std::move(that.value));
               break;

            case symbol_kind::S_func_application: // func_application
            case symbol_kind::S_func_expr: // func_expr
            case symbol_kind::S_func_expr_common_subexpr: // func_expr_common_subexpr
               value.move<std::shared_ptr<lingodb::ast::FunctionExpression>>(std::move(that.value));
               break;

            case symbol_kind::S_group_clause: // group_clause
            case symbol_kind::S_group_clause_with_alias: // group_clause_with_alias
               value.move<std::shared_ptr<lingodb::ast::GroupByNode>>(std::move(that.value));
               break;

            case symbol_kind::S_InsertStmt: // InsertStmt
            case symbol_kind::S_insert_rest: // insert_rest
               value.move<std::shared_ptr<lingodb::ast::InsertNode>>(std::move(that.value));
               break;

            case symbol_kind::S_select_limit: // select_limit
            case symbol_kind::S_limit_clause: // limit_clause
               value.move<std::shared_ptr<lingodb::ast::LimitModifier>>(std::move(that.value));
               break;

            case symbol_kind::S_sortby: // sortby
               value.move<std::shared_ptr<lingodb::ast::OrderByElement>>(std::move(that.value));
               break;

            case symbol_kind::S_sort_clause: // sort_clause
               value.move<std::shared_ptr<lingodb::ast::OrderByModifier>>(std::move(that.value));
               break;

            case symbol_kind::S_var_value: // var_value
            case symbol_kind::S_opt_boolean_or_string: // opt_boolean_or_string
            case symbol_kind::S_NonReservedWord_or_Sconst: // NonReservedWord_or_Sconst
            case symbol_kind::S_group_by_item: // group_by_item
            case symbol_kind::S_group_by_item_with_alias: // group_by_item_with_alias
            case symbol_kind::S_having_clause: // having_clause
            case symbol_kind::S_offset_clause: // offset_clause
            case symbol_kind::S_select_limit_value: // select_limit_value
            case symbol_kind::S_select_offset_value: // select_offset_value
            case symbol_kind::S_where_clause: // where_clause
            case symbol_kind::S_a_expr: // a_expr
            case symbol_kind::S_b_expr: // b_expr
            case symbol_kind::S_c_expr: // c_expr
            case symbol_kind::S_case_expr: // case_expr
            case symbol_kind::S_case_default: // case_default
            case symbol_kind::S_columnref: // columnref
            case symbol_kind::S_func_arg_expr: // func_arg_expr
            case symbol_kind::S_cast_expr: // cast_expr
            case symbol_kind::S_indirection: // indirection
            case symbol_kind::S_indirection_el: // indirection_el
            case symbol_kind::S_target_el: // target_el
            case symbol_kind::S_NumericOnly: // NumericOnly
            case symbol_kind::S_AexprConst: // AexprConst
            case symbol_kind::S_Iconst: // Iconst
            case symbol_kind::S_SignedIconst: // SignedIconst
            case symbol_kind::S_Fconst: // Fconst
            case symbol_kind::S_Sconst: // Sconst
            case symbol_kind::S_Bconst: // Bconst
               value.move<std::shared_ptr<lingodb::ast::ParsedExpression>>(std::move(that.value));
               break;

            case symbol_kind::S_pipe_start: // pipe_start
            case symbol_kind::S_pipe_operator: // pipe_operator
               value.move<std::shared_ptr<lingodb::ast::PipeOperator>>(std::move(that.value));
               break;

            case symbol_kind::S_select_with_parens: // select_with_parens
            case symbol_kind::S_select_no_parens: // select_no_parens
            case symbol_kind::S_select_clause: // select_clause
            case symbol_kind::S_simple_select: // simple_select
            case symbol_kind::S_with_clause: // with_clause
            case symbol_kind::S_cte_list: // cte_list
            case symbol_kind::S_common_table_expr: // common_table_expr
               value.move<std::shared_ptr<lingodb::ast::QueryNode>>(std::move(that.value));
               break;

            case symbol_kind::S_set_list: // set_list
               value.move<std::shared_ptr<lingodb::ast::SetColumnExpression>>(std::move(that.value));
               break;

            case symbol_kind::S_TableElement: // TableElement
            case symbol_kind::S_TableConstraint: // TableConstraint
            case symbol_kind::S_columnElement: // columnElement
               value.move<std::shared_ptr<lingodb::ast::TableElement>>(std::move(that.value));
               break;

            case symbol_kind::S_SelectStmt: // SelectStmt
            case symbol_kind::S_PipeSQLStmt: // PipeSQLStmt
            case symbol_kind::S_classic_select_and_pipe_sql_with_parens: // classic_select_and_pipe_sql_with_parens
            case symbol_kind::S_pipe_sql_with_parens: // pipe_sql_with_parens
            case symbol_kind::S_pipe_sql_no_parens: // pipe_sql_no_parens
            case symbol_kind::S_pipe_or_select_clause: // pipe_or_select_clause
            case symbol_kind::S_PreparableStmt: // PreparableStmt
               value.move<std::shared_ptr<lingodb::ast::TableProducer>>(std::move(that.value));
               break;

            case symbol_kind::S_opt_from_clause: // opt_from_clause
            case symbol_kind::S_from_clause: // from_clause
            case symbol_kind::S_table_ref: // table_ref
            case symbol_kind::S_joined_table: // joined_table
               value.move<std::shared_ptr<lingodb::ast::TableRef>>(std::move(that.value));
               break;

            case symbol_kind::S_opt_target_list: // opt_target_list
               value.move<std::shared_ptr<lingodb::ast::TargetList>>(std::move(that.value));
               break;

            case symbol_kind::S_type_modifier: // type_modifier
               value.move<std::shared_ptr<lingodb::ast::Value>>(std::move(that.value));
               break;

            case symbol_kind::S_window_func_expr: // window_func_expr
            case symbol_kind::S_over_clause: // over_clause
            case symbol_kind::S_window_specification: // window_specification
               value.move<std::shared_ptr<lingodb::ast::WindowExpression>>(std::move(that.value));
               break;

            case symbol_kind::S_opt_frame_clause: // opt_frame_clause
            case symbol_kind::S_frame_extent: // frame_extent
            case symbol_kind::S_frame_bound: // frame_bound
               value.move<std::shared_ptr<lingodb::ast::WindowFrame>>(std::move(that.value));
               break;

            case symbol_kind::S_FCONST: // FCONST
            case symbol_kind::S_IDENT: // IDENT
            case symbol_kind::S_IDENTIFIER: // "identifier"
            case symbol_kind::S_STRING_VALUE: // STRING_VALUE
            case symbol_kind::S_BIT_VALUE: // "bit_string"
            case symbol_kind::S_HEX_VALUE: // "hex_string"
            case symbol_kind::S_NATIONAL_VALUE: // "nat_string"
            case symbol_kind::S_PERCENT: // "%"
            case symbol_kind::S_PLUS: // "+"
            case symbol_kind::S_SLASH: // "/"
            case symbol_kind::S_STAR: // "*"
            case symbol_kind::S_MINUS: // "-"
            case symbol_kind::S_EQUAL: // "="
            case symbol_kind::S_NOT_EQUAL: // "<>"
            case symbol_kind::S_LESS_EQUAL: // "<="
            case symbol_kind::S_LESS: // "<"
            case symbol_kind::S_GREATER_EQUAL: // ">="
            case symbol_kind::S_GREATER: // ">"
            case symbol_kind::S_UIDENT: // UIDENT
            case symbol_kind::S_SCONST: // SCONST
            case symbol_kind::S_USCONST: // USCONST
            case symbol_kind::S_BCONST: // BCONST
            case symbol_kind::S_XCONST: // XCONST
            case symbol_kind::S_Op: // Op
            case symbol_kind::S_TYPECAST: // TYPECAST
            case symbol_kind::S_ABORT_P: // ABORT_P
            case symbol_kind::S_ABSENT: // ABSENT
            case symbol_kind::S_ABSOLUTE_P: // ABSOLUTE_P
            case symbol_kind::S_ACCESS: // ACCESS
            case symbol_kind::S_ACTION: // ACTION
            case symbol_kind::S_ADD_P: // ADD_P
            case symbol_kind::S_ADMIN: // ADMIN
            case symbol_kind::S_AFTER: // AFTER
            case symbol_kind::S_AGGREGATE: // AGGREGATE
            case symbol_kind::S_ALL: // ALL
            case symbol_kind::S_ALSO: // ALSO
            case symbol_kind::S_ALTER: // ALTER
            case symbol_kind::S_ALWAYS: // ALWAYS
            case symbol_kind::S_ANALYSE: // ANALYSE
            case symbol_kind::S_ANALYZE: // ANALYZE
            case symbol_kind::S_AND: // AND
            case symbol_kind::S_ANY: // ANY
            case symbol_kind::S_ARRAY: // ARRAY
            case symbol_kind::S_AS: // AS
            case symbol_kind::S_ASC: // ASC
            case symbol_kind::S_ASENSITIVE: // ASENSITIVE
            case symbol_kind::S_ASSERTION: // ASSERTION
            case symbol_kind::S_ASSIGNMENT: // ASSIGNMENT
            case symbol_kind::S_ASYMMETRIC: // ASYMMETRIC
            case symbol_kind::S_ATOMIC: // ATOMIC
            case symbol_kind::S_AT: // AT
            case symbol_kind::S_ATTACH: // ATTACH
            case symbol_kind::S_ATTRIBUTE: // ATTRIBUTE
            case symbol_kind::S_AUTHORIZATION: // AUTHORIZATION
            case symbol_kind::S_BACKWARD: // BACKWARD
            case symbol_kind::S_BEFORE: // BEFORE
            case symbol_kind::S_BEGIN_P: // BEGIN_P
            case symbol_kind::S_BETWEEN: // BETWEEN
            case symbol_kind::S_BIGINT: // BIGINT
            case symbol_kind::S_BINARY: // BINARY
            case symbol_kind::S_BIT: // BIT
            case symbol_kind::S_BOOLEAN_P: // BOOLEAN_P
            case symbol_kind::S_BOTH: // BOTH
            case symbol_kind::S_BREADTH: // BREADTH
            case symbol_kind::S_BY: // BY
            case symbol_kind::S_CACHE: // CACHE
            case symbol_kind::S_CALL: // CALL
            case symbol_kind::S_CALLED: // CALLED
            case symbol_kind::S_CASCADE: // CASCADE
            case symbol_kind::S_CASCADED: // CASCADED
            case symbol_kind::S_CASE: // CASE
            case symbol_kind::S_CAST: // CAST
            case symbol_kind::S_CATALOG_P: // CATALOG_P
            case symbol_kind::S_CHAIN: // CHAIN
            case symbol_kind::S_CHAR_P: // CHAR_P
            case symbol_kind::S_CHARACTER: // CHARACTER
            case symbol_kind::S_CHARACTERISTICS: // CHARACTERISTICS
            case symbol_kind::S_CHECK: // CHECK
            case symbol_kind::S_CHECKPOINT: // CHECKPOINT
            case symbol_kind::S_CLASS: // CLASS
            case symbol_kind::S_CLOSE: // CLOSE
            case symbol_kind::S_CLUSTER: // CLUSTER
            case symbol_kind::S_COALESCE: // COALESCE
            case symbol_kind::S_COLLATE: // COLLATE
            case symbol_kind::S_COLLATION: // COLLATION
            case symbol_kind::S_COLUMN: // COLUMN
            case symbol_kind::S_COLUMNS: // COLUMNS
            case symbol_kind::S_COMMENT: // COMMENT
            case symbol_kind::S_COMMENTS: // COMMENTS
            case symbol_kind::S_COMMIT: // COMMIT
            case symbol_kind::S_COMMITTED: // COMMITTED
            case symbol_kind::S_COMPRESSION: // COMPRESSION
            case symbol_kind::S_CONCURRENTLY: // CONCURRENTLY
            case symbol_kind::S_CONDITIONAL: // CONDITIONAL
            case symbol_kind::S_CONFIGURATION: // CONFIGURATION
            case symbol_kind::S_CONFLICT: // CONFLICT
            case symbol_kind::S_CONNECTION: // CONNECTION
            case symbol_kind::S_CONSTRAINT: // CONSTRAINT
            case symbol_kind::S_CONSTRAINTS: // CONSTRAINTS
            case symbol_kind::S_CONTENT_P: // CONTENT_P
            case symbol_kind::S_CONTINUE_P: // CONTINUE_P
            case symbol_kind::S_CONVERSION_P: // CONVERSION_P
            case symbol_kind::S_COPY: // COPY
            case symbol_kind::S_COST: // COST
            case symbol_kind::S_CREATE: // CREATE
            case symbol_kind::S_CROSS: // CROSS
            case symbol_kind::S_CSV: // CSV
            case symbol_kind::S_CUBE: // CUBE
            case symbol_kind::S_CURRENT_P: // CURRENT_P
            case symbol_kind::S_CURRENT_CATALOG: // CURRENT_CATALOG
            case symbol_kind::S_CURRENT_DATE: // CURRENT_DATE
            case symbol_kind::S_CURRENT_ROLE: // CURRENT_ROLE
            case symbol_kind::S_CURRENT_SCHEMA: // CURRENT_SCHEMA
            case symbol_kind::S_CURRENT_TIME: // CURRENT_TIME
            case symbol_kind::S_CURRENT_TIMESTAMP: // CURRENT_TIMESTAMP
            case symbol_kind::S_CURRENT_USER: // CURRENT_USER
            case symbol_kind::S_CURSOR: // CURSOR
            case symbol_kind::S_CYCLE: // CYCLE
            case symbol_kind::S_DATA_P: // DATA_P
            case symbol_kind::S_DATABASE: // DATABASE
            case symbol_kind::S_DAY_P: // DAY_P
            case symbol_kind::S_DEALLOCATE: // DEALLOCATE
            case symbol_kind::S_DEC: // DEC
            case symbol_kind::S_DECIMAL_P: // DECIMAL_P
            case symbol_kind::S_DECLARE: // DECLARE
            case symbol_kind::S_DEFAULT: // DEFAULT
            case symbol_kind::S_DEFAULTS: // DEFAULTS
            case symbol_kind::S_DATE_P: // DATE_P
            case symbol_kind::S_DEFERRABLE: // DEFERRABLE
            case symbol_kind::S_DEFERRED: // DEFERRED
            case symbol_kind::S_DEFINER: // DEFINER
            case symbol_kind::S_DELETE_P: // DELETE_P
            case symbol_kind::S_DELIMITER: // DELIMITER
            case symbol_kind::S_DELIMITERS: // DELIMITERS
            case symbol_kind::S_DEPENDS: // DEPENDS
            case symbol_kind::S_DEPTH: // DEPTH
            case symbol_kind::S_DESC: // DESC
            case symbol_kind::S_DETACH: // DETACH
            case symbol_kind::S_DICTIONARY: // DICTIONARY
            case symbol_kind::S_DISABLE_P: // DISABLE_P
            case symbol_kind::S_DISCARD: // DISCARD
            case symbol_kind::S_DISTINCT: // DISTINCT
            case symbol_kind::S_DO: // DO
            case symbol_kind::S_DOCUMENT_P: // DOCUMENT_P
            case symbol_kind::S_DOMAIN_P: // DOMAIN_P
            case symbol_kind::S_DOUBLE_P: // DOUBLE_P
            case symbol_kind::S_DROP: // DROP
            case symbol_kind::S_EACH: // EACH
            case symbol_kind::S_ELSE: // ELSE
            case symbol_kind::S_EMPTY_P: // EMPTY_P
            case symbol_kind::S_ENABLE_P: // ENABLE_P
            case symbol_kind::S_ENCODING: // ENCODING
            case symbol_kind::S_ENCRYPTED: // ENCRYPTED
            case symbol_kind::S_END_P: // END_P
            case symbol_kind::S_ENFORCED: // ENFORCED
            case symbol_kind::S_ENUM_P: // ENUM_P
            case symbol_kind::S_ERROR_P: // ERROR_P
            case symbol_kind::S_ESCAPE: // ESCAPE
            case symbol_kind::S_EVENT: // EVENT
            case symbol_kind::S_EXCEPT: // EXCEPT
            case symbol_kind::S_EXCLUDE: // EXCLUDE
            case symbol_kind::S_EXCLUDING: // EXCLUDING
            case symbol_kind::S_EXCLUSIVE: // EXCLUSIVE
            case symbol_kind::S_EXECUTE: // EXECUTE
            case symbol_kind::S_EXISTS: // EXISTS
            case symbol_kind::S_EXPLAIN: // EXPLAIN
            case symbol_kind::S_EXPRESSION: // EXPRESSION
            case symbol_kind::S_EXTENSION: // EXTENSION
            case symbol_kind::S_EXTERNAL: // EXTERNAL
            case symbol_kind::S_EXTRACT: // EXTRACT
            case symbol_kind::S_FALSE_P: // FALSE_P
            case symbol_kind::S_FAMILY: // FAMILY
            case symbol_kind::S_FETCH: // FETCH
            case symbol_kind::S_FILTER: // FILTER
            case symbol_kind::S_FINALIZE: // FINALIZE
            case symbol_kind::S_FIRST_P: // FIRST_P
            case symbol_kind::S_FLOAT_P: // FLOAT_P
            case symbol_kind::S_FOLLOWING: // FOLLOWING
            case symbol_kind::S_FOR: // FOR
            case symbol_kind::S_FORCE: // FORCE
            case symbol_kind::S_FOREIGN: // FOREIGN
            case symbol_kind::S_FORMAT: // FORMAT
            case symbol_kind::S_FORWARD: // FORWARD
            case symbol_kind::S_FREEZE: // FREEZE
            case symbol_kind::S_FROM: // FROM
            case symbol_kind::S_FULL: // FULL
            case symbol_kind::S_FUNCTION: // FUNCTION
            case symbol_kind::S_FUNCTIONS: // FUNCTIONS
            case symbol_kind::S_GENERATED: // GENERATED
            case symbol_kind::S_GLOBAL: // GLOBAL
            case symbol_kind::S_GRANT: // GRANT
            case symbol_kind::S_GRANTED: // GRANTED
            case symbol_kind::S_GREATEST: // GREATEST
            case symbol_kind::S_GROUP_P: // GROUP_P
            case symbol_kind::S_GROUPING: // GROUPING
            case symbol_kind::S_GROUPS: // GROUPS
            case symbol_kind::S_HANDLER: // HANDLER
            case symbol_kind::S_HAVING: // HAVING
            case symbol_kind::S_HEADER_P: // HEADER_P
            case symbol_kind::S_HOLD: // HOLD
            case symbol_kind::S_HOUR_P: // HOUR_P
            case symbol_kind::S_IDENTITY_P: // IDENTITY_P
            case symbol_kind::S_IF_P: // IF_P
            case symbol_kind::S_ILIKE: // ILIKE
            case symbol_kind::S_IMMEDIATE: // IMMEDIATE
            case symbol_kind::S_IMMUTABLE: // IMMUTABLE
            case symbol_kind::S_IMPLICIT_P: // IMPLICIT_P
            case symbol_kind::S_IMPORT_P: // IMPORT_P
            case symbol_kind::S_IN_P: // IN_P
            case symbol_kind::S_INCLUDE: // INCLUDE
            case symbol_kind::S_INCLUDING: // INCLUDING
            case symbol_kind::S_INCREMENT: // INCREMENT
            case symbol_kind::S_INDENT: // INDENT
            case symbol_kind::S_INDEX: // INDEX
            case symbol_kind::S_INDEXES: // INDEXES
            case symbol_kind::S_INHERIT: // INHERIT
            case symbol_kind::S_INHERITS: // INHERITS
            case symbol_kind::S_INITIALLY: // INITIALLY
            case symbol_kind::S_INLINE_P: // INLINE_P
            case symbol_kind::S_INNER_P: // INNER_P
            case symbol_kind::S_INOUT: // INOUT
            case symbol_kind::S_INPUT_P: // INPUT_P
            case symbol_kind::S_INSENSITIVE: // INSENSITIVE
            case symbol_kind::S_INSERT: // INSERT
            case symbol_kind::S_INSTEAD: // INSTEAD
            case symbol_kind::S_INT_P: // INT_P
            case symbol_kind::S_INTEGER: // INTEGER
            case symbol_kind::S_INTERSECT: // INTERSECT
            case symbol_kind::S_INTERVAL: // INTERVAL
            case symbol_kind::S_INTO: // INTO
            case symbol_kind::S_INVOKER: // INVOKER
            case symbol_kind::S_IS: // IS
            case symbol_kind::S_ISNULL: // ISNULL
            case symbol_kind::S_ISOLATION: // ISOLATION
            case symbol_kind::S_JOIN: // JOIN
            case symbol_kind::S_JSON: // JSON
            case symbol_kind::S_JSON_ARRAY: // JSON_ARRAY
            case symbol_kind::S_JSON_ARRAYAGG: // JSON_ARRAYAGG
            case symbol_kind::S_JSON_EXISTS: // JSON_EXISTS
            case symbol_kind::S_JSON_OBJECT: // JSON_OBJECT
            case symbol_kind::S_JSON_OBJECTAGG: // JSON_OBJECTAGG
            case symbol_kind::S_JSON_QUERY: // JSON_QUERY
            case symbol_kind::S_JSON_SCALAR: // JSON_SCALAR
            case symbol_kind::S_JSON_SERIALIZE: // JSON_SERIALIZE
            case symbol_kind::S_JSON_TABLE: // JSON_TABLE
            case symbol_kind::S_JSON_VALUE: // JSON_VALUE
            case symbol_kind::S_KEEP: // KEEP
            case symbol_kind::S_KEY: // KEY
            case symbol_kind::S_KEYS: // KEYS
            case symbol_kind::S_LABEL: // LABEL
            case symbol_kind::S_LANGUAGE: // LANGUAGE
            case symbol_kind::S_LARGE_P: // LARGE_P
            case symbol_kind::S_LAST_P: // LAST_P
            case symbol_kind::S_LATERAL_P: // LATERAL_P
            case symbol_kind::S_LEADING: // LEADING
            case symbol_kind::S_LEAKPROOF: // LEAKPROOF
            case symbol_kind::S_LEAST: // LEAST
            case symbol_kind::S_LEFT: // LEFT
            case symbol_kind::S_LEVEL: // LEVEL
            case symbol_kind::S_LIKE: // LIKE
            case symbol_kind::S_LIMIT: // LIMIT
            case symbol_kind::S_LISTEN: // LISTEN
            case symbol_kind::S_LOAD: // LOAD
            case symbol_kind::S_LOCAL: // LOCAL
            case symbol_kind::S_LOCALTIME: // LOCALTIME
            case symbol_kind::S_LOCALTIMESTAMP: // LOCALTIMESTAMP
            case symbol_kind::S_LOCATION: // LOCATION
            case symbol_kind::S_LOCK_P: // LOCK_P
            case symbol_kind::S_LOCKED: // LOCKED
            case symbol_kind::S_LOGGED: // LOGGED
            case symbol_kind::S_MAPPING: // MAPPING
            case symbol_kind::S_MATCH: // MATCH
            case symbol_kind::S_MATCHED: // MATCHED
            case symbol_kind::S_MATERIALIZED: // MATERIALIZED
            case symbol_kind::S_MAXVALUE: // MAXVALUE
            case symbol_kind::S_MERGE: // MERGE
            case symbol_kind::S_MERGE_ACTION: // MERGE_ACTION
            case symbol_kind::S_METHOD: // METHOD
            case symbol_kind::S_MINUTE_P: // MINUTE_P
            case symbol_kind::S_MINVALUE: // MINVALUE
            case symbol_kind::S_MODE: // MODE
            case symbol_kind::S_MONTH_P: // MONTH_P
            case symbol_kind::S_MOVE: // MOVE
            case symbol_kind::S_NAME_P: // NAME_P
            case symbol_kind::S_NAMES: // NAMES
            case symbol_kind::S_NATIONAL: // NATIONAL
            case symbol_kind::S_NATURAL: // NATURAL
            case symbol_kind::S_NCHAR: // NCHAR
            case symbol_kind::S_NESTED: // NESTED
            case symbol_kind::S_NEW: // NEW
            case symbol_kind::S_NEXT: // NEXT
            case symbol_kind::S_NFC: // NFC
            case symbol_kind::S_NFD: // NFD
            case symbol_kind::S_NFKC: // NFKC
            case symbol_kind::S_NFKD: // NFKD
            case symbol_kind::S_NO: // NO
            case symbol_kind::S_NONE: // NONE
            case symbol_kind::S_NORMALIZE: // NORMALIZE
            case symbol_kind::S_NORMALIZED: // NORMALIZED
            case symbol_kind::S_NOT: // NOT
            case symbol_kind::S_NOTHING: // NOTHING
            case symbol_kind::S_NOTIFY: // NOTIFY
            case symbol_kind::S_NOTNULL: // NOTNULL
            case symbol_kind::S_NOWAIT: // NOWAIT
            case symbol_kind::S_NULL_P: // NULL_P
            case symbol_kind::S_NULLIF: // NULLIF
            case symbol_kind::S_NULLS_P: // NULLS_P
            case symbol_kind::S_NUMERIC: // NUMERIC
            case symbol_kind::S_OBJECT_P: // OBJECT_P
            case symbol_kind::S_OBJECTS_P: // OBJECTS_P
            case symbol_kind::S_OF: // OF
            case symbol_kind::S_OFF: // OFF
            case symbol_kind::S_OFFSET: // OFFSET
            case symbol_kind::S_OIDS: // OIDS
            case symbol_kind::S_OLD: // OLD
            case symbol_kind::S_OMIT: // OMIT
            case symbol_kind::S_ON: // ON
            case symbol_kind::S_ONLY: // ONLY
            case symbol_kind::S_OPERATOR: // OPERATOR
            case symbol_kind::S_OPTION: // OPTION
            case symbol_kind::S_OPTIONS: // OPTIONS
            case symbol_kind::S_OR: // OR
            case symbol_kind::S_ORDER: // ORDER
            case symbol_kind::S_ORDINALITY: // ORDINALITY
            case symbol_kind::S_OTHERS: // OTHERS
            case symbol_kind::S_OUT_P: // OUT_P
            case symbol_kind::S_OUTER_P: // OUTER_P
            case symbol_kind::S_OVER: // OVER
            case symbol_kind::S_OVERLAPS: // OVERLAPS
            case symbol_kind::S_OVERLAY: // OVERLAY
            case symbol_kind::S_OVERRIDING: // OVERRIDING
            case symbol_kind::S_OWNED: // OWNED
            case symbol_kind::S_OWNER: // OWNER
            case symbol_kind::S_PARALLEL: // PARALLEL
            case symbol_kind::S_PARAMETER: // PARAMETER
            case symbol_kind::S_PARSER: // PARSER
            case symbol_kind::S_PARTIAL: // PARTIAL
            case symbol_kind::S_PARTITION: // PARTITION
            case symbol_kind::S_PASSING: // PASSING
            case symbol_kind::S_PASSWORD: // PASSWORD
            case symbol_kind::S_PATH: // PATH
            case symbol_kind::S_PERIOD: // PERIOD
            case symbol_kind::S_PLACING: // PLACING
            case symbol_kind::S_PLAN: // PLAN
            case symbol_kind::S_PLANS: // PLANS
            case symbol_kind::S_POLICY: // POLICY
            case symbol_kind::S_POSITION: // POSITION
            case symbol_kind::S_PRECEDING: // PRECEDING
            case symbol_kind::S_PRECISION: // PRECISION
            case symbol_kind::S_PRESERVE: // PRESERVE
            case symbol_kind::S_PREPARE: // PREPARE
            case symbol_kind::S_PREPARED: // PREPARED
            case symbol_kind::S_PRIMARY: // PRIMARY
            case symbol_kind::S_PRIOR: // PRIOR
            case symbol_kind::S_PRIVILEGES: // PRIVILEGES
            case symbol_kind::S_PROCEDURAL: // PROCEDURAL
            case symbol_kind::S_PROCEDURE: // PROCEDURE
            case symbol_kind::S_PROCEDURES: // PROCEDURES
            case symbol_kind::S_PROGRAM: // PROGRAM
            case symbol_kind::S_PUBLICATION: // PUBLICATION
            case symbol_kind::S_QUOTES: // QUOTES
            case symbol_kind::S_EXTEND: // EXTEND
            case symbol_kind::S_RANGE: // RANGE
            case symbol_kind::S_READ: // READ
            case symbol_kind::S_REAL: // REAL
            case symbol_kind::S_REASSIGN: // REASSIGN
            case symbol_kind::S_RECURSIVE: // RECURSIVE
            case symbol_kind::S_REF_P: // REF_P
            case symbol_kind::S_REFERENCES: // REFERENCES
            case symbol_kind::S_REFERENCING: // REFERENCING
            case symbol_kind::S_REFRESH: // REFRESH
            case symbol_kind::S_REINDEX: // REINDEX
            case symbol_kind::S_RELATIVE_P: // RELATIVE_P
            case symbol_kind::S_RELEASE: // RELEASE
            case symbol_kind::S_RENAME: // RENAME
            case symbol_kind::S_REPEATABLE: // REPEATABLE
            case symbol_kind::S_REPLACE: // REPLACE
            case symbol_kind::S_REPLICA: // REPLICA
            case symbol_kind::S_RESET: // RESET
            case symbol_kind::S_RESTART: // RESTART
            case symbol_kind::S_RESTRICT: // RESTRICT
            case symbol_kind::S_RETURN: // RETURN
            case symbol_kind::S_RETURNING: // RETURNING
            case symbol_kind::S_RETURNS: // RETURNS
            case symbol_kind::S_REVOKE: // REVOKE
            case symbol_kind::S_RIGHT: // RIGHT
            case symbol_kind::S_ROLE: // ROLE
            case symbol_kind::S_ROLLBACK: // ROLLBACK
            case symbol_kind::S_ROLLUP: // ROLLUP
            case symbol_kind::S_ROUTINE: // ROUTINE
            case symbol_kind::S_ROUTINES: // ROUTINES
            case symbol_kind::S_ROW: // ROW
            case symbol_kind::S_ROWS: // ROWS
            case symbol_kind::S_RULE: // RULE
            case symbol_kind::S_SAVEPOINT: // SAVEPOINT
            case symbol_kind::S_SCALAR: // SCALAR
            case symbol_kind::S_SCHEMA: // SCHEMA
            case symbol_kind::S_SCHEMAS: // SCHEMAS
            case symbol_kind::S_SCROLL: // SCROLL
            case symbol_kind::S_SEARCH: // SEARCH
            case symbol_kind::S_SECOND_P: // SECOND_P
            case symbol_kind::S_SECURITY: // SECURITY
            case symbol_kind::S_SELECT: // SELECT
            case symbol_kind::S_SEQUENCE: // SEQUENCE
            case symbol_kind::S_SEQUENCES: // SEQUENCES
            case symbol_kind::S_SERIALIZABLE: // SERIALIZABLE
            case symbol_kind::S_SERVER: // SERVER
            case symbol_kind::S_SESSION: // SESSION
            case symbol_kind::S_SESSION_USER: // SESSION_USER
            case symbol_kind::S_SET: // SET
            case symbol_kind::S_SETS: // SETS
            case symbol_kind::S_SETOF: // SETOF
            case symbol_kind::S_SHARE: // SHARE
            case symbol_kind::S_SHOW: // SHOW
            case symbol_kind::S_SIMILAR: // SIMILAR
            case symbol_kind::S_SIMPLE: // SIMPLE
            case symbol_kind::S_SKIP: // SKIP
            case symbol_kind::S_SMALLINT: // SMALLINT
            case symbol_kind::S_SNAPSHOT: // SNAPSHOT
            case symbol_kind::S_SOME: // SOME
            case symbol_kind::S_SOURCE: // SOURCE
            case symbol_kind::S_SQL_P: // SQL_P
            case symbol_kind::S_STABLE: // STABLE
            case symbol_kind::S_STANDALONE_P: // STANDALONE_P
            case symbol_kind::S_START: // START
            case symbol_kind::S_STATEMENT: // STATEMENT
            case symbol_kind::S_STATISTICS: // STATISTICS
            case symbol_kind::S_STDIN: // STDIN
            case symbol_kind::S_STDOUT: // STDOUT
            case symbol_kind::S_STORAGE: // STORAGE
            case symbol_kind::S_STORED: // STORED
            case symbol_kind::S_STRICT_P: // STRICT_P
            case symbol_kind::S_STRING_P: // STRING_P
            case symbol_kind::S_STRIP_P: // STRIP_P
            case symbol_kind::S_SUBSCRIPTION: // SUBSCRIPTION
            case symbol_kind::S_SUBSTRING: // SUBSTRING
            case symbol_kind::S_SUPPORT: // SUPPORT
            case symbol_kind::S_SYMMETRIC: // SYMMETRIC
            case symbol_kind::S_SYSID: // SYSID
            case symbol_kind::S_SYSTEM_P: // SYSTEM_P
            case symbol_kind::S_SYSTEM_USER: // SYSTEM_USER
            case symbol_kind::S_TABLE: // TABLE
            case symbol_kind::S_TABLES: // TABLES
            case symbol_kind::S_TABLESAMPLE: // TABLESAMPLE
            case symbol_kind::S_TABLESPACE: // TABLESPACE
            case symbol_kind::S_TARGET: // TARGET
            case symbol_kind::S_TEMP: // TEMP
            case symbol_kind::S_TEMPLATE: // TEMPLATE
            case symbol_kind::S_TEMPORARY: // TEMPORARY
            case symbol_kind::S_TEXT_P: // TEXT_P
            case symbol_kind::S_THEN: // THEN
            case symbol_kind::S_TIES: // TIES
            case symbol_kind::S_TIME: // TIME
            case symbol_kind::S_TIMESTAMP: // TIMESTAMP
            case symbol_kind::S_TO: // TO
            case symbol_kind::S_TRAILING: // TRAILING
            case symbol_kind::S_TRANSACTION: // TRANSACTION
            case symbol_kind::S_TRANSFORM: // TRANSFORM
            case symbol_kind::S_TREAT: // TREAT
            case symbol_kind::S_TRIGGER: // TRIGGER
            case symbol_kind::S_TRIM: // TRIM
            case symbol_kind::S_TRUE_P: // TRUE_P
            case symbol_kind::S_TRUNCATE: // TRUNCATE
            case symbol_kind::S_TRUSTED: // TRUSTED
            case symbol_kind::S_TYPE_P: // TYPE_P
            case symbol_kind::S_TYPES_P: // TYPES_P
            case symbol_kind::S_UESCAPE: // UESCAPE
            case symbol_kind::S_UNBOUNDED: // UNBOUNDED
            case symbol_kind::S_UNCONDITIONAL: // UNCONDITIONAL
            case symbol_kind::S_UNCOMMITTED: // UNCOMMITTED
            case symbol_kind::S_UNENCRYPTED: // UNENCRYPTED
            case symbol_kind::S_UNION: // UNION
            case symbol_kind::S_UNIQUE: // UNIQUE
            case symbol_kind::S_UNKNOWN: // UNKNOWN
            case symbol_kind::S_UNLISTEN: // UNLISTEN
            case symbol_kind::S_UNLOGGED: // UNLOGGED
            case symbol_kind::S_UNTIL: // UNTIL
            case symbol_kind::S_UPDATE: // UPDATE
            case symbol_kind::S_USER: // USER
            case symbol_kind::S_USING: // USING
            case symbol_kind::S_VACUUM: // VACUUM
            case symbol_kind::S_VALID: // VALID
            case symbol_kind::S_VALIDATE: // VALIDATE
            case symbol_kind::S_VALIDATOR: // VALIDATOR
            case symbol_kind::S_VALUE_P: // VALUE_P
            case symbol_kind::S_VALUES: // VALUES
            case symbol_kind::S_VARCHAR: // VARCHAR
            case symbol_kind::S_VARIADIC: // VARIADIC
            case symbol_kind::S_VARYING: // VARYING
            case symbol_kind::S_VERBOSE: // VERBOSE
            case symbol_kind::S_VERSION_P: // VERSION_P
            case symbol_kind::S_VIEW: // VIEW
            case symbol_kind::S_VIEWS: // VIEWS
            case symbol_kind::S_VIRTUAL: // VIRTUAL
            case symbol_kind::S_VOLATILE: // VOLATILE
            case symbol_kind::S_WHEN: // WHEN
            case symbol_kind::S_WHERE: // WHERE
            case symbol_kind::S_WHITESPACE_P: // WHITESPACE_P
            case symbol_kind::S_WINDOW: // WINDOW
            case symbol_kind::S_WITH: // WITH
            case symbol_kind::S_WITHIN: // WITHIN
            case symbol_kind::S_WITHOUT: // WITHOUT
            case symbol_kind::S_WORK: // WORK
            case symbol_kind::S_WRAPPER: // WRAPPER
            case symbol_kind::S_WRITE: // WRITE
            case symbol_kind::S_XML_P: // XML_P
            case symbol_kind::S_XMLATTRIBUTES: // XMLATTRIBUTES
            case symbol_kind::S_XMLCONCAT: // XMLCONCAT
            case symbol_kind::S_XMLELEMENT: // XMLELEMENT
            case symbol_kind::S_XMLEXISTS: // XMLEXISTS
            case symbol_kind::S_XMLFOREST: // XMLFOREST
            case symbol_kind::S_XMLNAMESPACES: // XMLNAMESPACES
            case symbol_kind::S_XMLPARSE: // XMLPARSE
            case symbol_kind::S_XMLPI: // XMLPI
            case symbol_kind::S_XMLROOT: // XMLROOT
            case symbol_kind::S_XMLSERIALIZE: // XMLSERIALIZE
            case symbol_kind::S_XMLTABLE: // XMLTABLE
            case symbol_kind::S_YEAR_P: // YEAR_P
            case symbol_kind::S_YES_P: // YES_P
            case symbol_kind::S_ZONE: // ZONE
            case symbol_kind::S_copy_file_name: // copy_file_name
            case symbol_kind::S_copy_delimiter: // copy_delimiter
            case symbol_kind::S_var_name: // var_name
            case symbol_kind::S_NonReservedWord: // NonReservedWord
            case symbol_kind::S_relation_expr: // relation_expr
            case symbol_kind::S_any_operator: // any_operator
            case symbol_kind::S_qual_Op: // qual_Op
            case symbol_kind::S_all_Op: // all_Op
            case symbol_kind::S_ColId: // ColId
            case symbol_kind::S_type_function_name: // type_function_name
            case symbol_kind::S_type_func_name_keyword: // type_func_name_keyword
            case symbol_kind::S_ColLabel: // ColLabel
            case symbol_kind::S_BareColLabel: // BareColLabel
            case symbol_kind::S_reserved_keyword: // reserved_keyword
            case symbol_kind::S_unreserved_keyword: // unreserved_keyword
            case symbol_kind::S_col_name_keyword: // col_name_keyword
            case symbol_kind::S_qualified_name: // qualified_name
            case symbol_kind::S_name: // name
            case symbol_kind::S_attr_name: // attr_name
            case symbol_kind::S_func_name: // func_name
            case symbol_kind::S_insert_target: // insert_target
            case symbol_kind::S_insert_column_item: // insert_column_item
               value.move<std::string>(std::move(that.value));
               break;

            case symbol_kind::S_in_expr: // in_expr
               value.move<std::variant<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>, std::shared_ptr<lingodb::ast::SubqueryExpression>>>(std::move(that.value));
               break;

            case symbol_kind::S_when_clause_list: // when_clause_list
               value.move<std::vector<lingodb::ast::CaseExpression::CaseCheck>>(std::move(that.value));
               break;

            case symbol_kind::S_copy_options: // copy_options
            case symbol_kind::S_copy_opt_list: // copy_opt_list
               value.move<std::vector<std::pair<std::string, std::string>>>(std::move(that.value));
               break;

            case symbol_kind::S_stmtmulti: // stmtmulti
               value.move<std::vector<std::shared_ptr<lingodb::ast::AstNode>>>(std::move(that.value));
               break;

            case symbol_kind::S_ColQualList: // ColQualList
               value.move<std::vector<std::shared_ptr<lingodb::ast::Constraint>>>(std::move(that.value));
               break;

            case symbol_kind::S_func_expr_list: // func_expr_list
               value.move<std::vector<std::shared_ptr<lingodb::ast::FunctionExpression>>>(std::move(that.value));
               break;

            case symbol_kind::S_sortby_list: // sortby_list
               value.move<std::vector<std::shared_ptr<lingodb::ast::OrderByElement>>>(std::move(that.value));
               break;

            case symbol_kind::S_var_list: // var_list
            case symbol_kind::S_group_by_list: // group_by_list
            case symbol_kind::S_group_by_list_with_alias: // group_by_list_with_alias
            case symbol_kind::S_rollup_clause: // rollup_clause
            case symbol_kind::S_columnref_list: // columnref_list
            case symbol_kind::S_func_arg_list_opt: // func_arg_list_opt
            case symbol_kind::S_func_arg_list: // func_arg_list
            case symbol_kind::S_expr_list: // expr_list
            case symbol_kind::S_expr_list_with_alias: // expr_list_with_alias
            case symbol_kind::S_extract_list: // extract_list
            case symbol_kind::S_substr_list: // substr_list
            case symbol_kind::S_target_list: // target_list
            case symbol_kind::S_opt_partition_clause: // opt_partition_clause
               value.move<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>>(std::move(that.value));
               break;

            case symbol_kind::S_OptTableElementList: // OptTableElementList
            case symbol_kind::S_TableElementList: // TableElementList
               value.move<std::vector<std::shared_ptr<lingodb::ast::TableElement>>>(std::move(that.value));
               break;

            case symbol_kind::S_opt_type_modifiers: // opt_type_modifiers
            case symbol_kind::S_type_modifiers: // type_modifiers
               value.move<std::vector<std::shared_ptr<lingodb::ast::Value>>>(std::move(that.value));
               break;

            case symbol_kind::S_opt_name_list: // opt_name_list
            case symbol_kind::S_name_list: // name_list
            case symbol_kind::S_insert_column_list: // insert_column_list
               value.move<std::vector<std::string>>(std::move(that.value));
               break;

            case symbol_kind::S_INTEGER_VALUE: // "integer_value"
               value.move<uint64_t>(std::move(that.value));
               break;

            default:
               break;
         }
      }
#endif

      /// Copy constructor.
      basic_symbol(const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, location_type&& l)
         : Base(t), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const location_type& l)
         : Base(t), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, bool&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const bool& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, int&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const int& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, lingodb::ast::CaseExpression::CaseCheck&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const lingodb::ast::CaseExpression::CaseCheck& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, lingodb::ast::ExpressionType&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const lingodb::ast::ExpressionType& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, lingodb::ast::JoinType&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const lingodb::ast::JoinType& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, lingodb::ast::LogicalTypeWithMods&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const lingodb::ast::LogicalTypeWithMods& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, lingodb::ast::OrderByNullType&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const lingodb::ast::OrderByNullType& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, lingodb::ast::OrderType&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const lingodb::ast::OrderType& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, lingodb::ast::SubqueryType&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const lingodb::ast::SubqueryType& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, lingodb::ast::jointCondOrUsingCols&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const lingodb::ast::jointCondOrUsingCols& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::optional<lingodb::ast::SQLAbstractLogicalType>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::optional<lingodb::ast::SQLAbstractLogicalType>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::optional<std::shared_ptr<lingodb::ast::OrderByModifier>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::optional<std::shared_ptr<lingodb::ast::OrderByModifier>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::optional<std::shared_ptr<lingodb::ast::ParsedExpression>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::optional<std::shared_ptr<lingodb::ast::ParsedExpression>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::optional<std::shared_ptr<lingodb::ast::ResultModifier>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::optional<std::shared_ptr<lingodb::ast::ResultModifier>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::pair<std::string, std::string>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::pair<std::string, std::string>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::pair<std::string, std::vector<std::string>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::pair<std::string, std::vector<std::string>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<ast::ConstantExpression>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<ast::ConstantExpression>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<ast::CopyNode>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<ast::CopyNode>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<ast::SetVariableStatement>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<ast::SetVariableStatement>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::AggregationNode>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::AggregationNode>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::AstNode>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::AstNode>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::ConjunctionExpression>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::ConjunctionExpression>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::Constraint>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::Constraint>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::CreateNode>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::CreateNode>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::CrossProductRef>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::CrossProductRef>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::ExpressionListRef>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::ExpressionListRef>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::FunctionExpression>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::FunctionExpression>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::GroupByNode>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::GroupByNode>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::InsertNode>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::InsertNode>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::LimitModifier>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::LimitModifier>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::OrderByElement>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::OrderByElement>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::OrderByModifier>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::OrderByModifier>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::ParsedExpression>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::ParsedExpression>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::PipeOperator>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::PipeOperator>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::QueryNode>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::QueryNode>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::SetColumnExpression>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::SetColumnExpression>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::TableElement>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::TableElement>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::TableProducer>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::TableProducer>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::TableRef>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::TableRef>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::TargetList>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::TargetList>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::Value>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::Value>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::WindowExpression>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::WindowExpression>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::shared_ptr<lingodb::ast::WindowFrame>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::shared_ptr<lingodb::ast::WindowFrame>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::string&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::string& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::variant<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>, std::shared_ptr<lingodb::ast::SubqueryExpression>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::variant<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>, std::shared_ptr<lingodb::ast::SubqueryExpression>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<lingodb::ast::CaseExpression::CaseCheck>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<lingodb::ast::CaseExpression::CaseCheck>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::pair<std::string, std::string>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::pair<std::string, std::string>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::shared_ptr<lingodb::ast::AstNode>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::shared_ptr<lingodb::ast::AstNode>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::shared_ptr<lingodb::ast::Constraint>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::shared_ptr<lingodb::ast::Constraint>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::shared_ptr<lingodb::ast::FunctionExpression>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::shared_ptr<lingodb::ast::FunctionExpression>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::shared_ptr<lingodb::ast::OrderByElement>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::shared_ptr<lingodb::ast::OrderByElement>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::shared_ptr<lingodb::ast::TableElement>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::shared_ptr<lingodb::ast::TableElement>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::shared_ptr<lingodb::ast::Value>>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::shared_ptr<lingodb::ast::Value>>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, std::vector<std::string>&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const std::vector<std::string>& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol(typename Base::kind_type t, uint64_t&& v, location_type&& l)
         : Base(t), value(std::move(v)), location(std::move(l)) {}
#else
      basic_symbol(typename Base::kind_type t, const uint64_t& v, const location_type& l)
         : Base(t), value(v), location(l) {}
#endif

      /// Destroy the symbol.
      ~basic_symbol() {
         clear();
      }

      /// Destroy contents, and record that is empty.
      void clear() YY_NOEXCEPT {
         // User destructor.
         symbol_kind_type yykind = this->kind();
         basic_symbol<Base>& yysym = *this;
         (void) yysym;
         switch (yykind) {
            default:
               break;
         }

         // Value type destructor.
         switch (yykind) {
            case symbol_kind::S_distinct_clause: // distinct_clause
            case symbol_kind::S_set_quantifier: // set_quantifier
            case symbol_kind::S_opt_asymmetric: // opt_asymmetric
            case symbol_kind::S_OptTemp: // OptTemp
            case symbol_kind::S_opt_varying: // opt_varying
               value.template destroy<bool>();
               break;

            case symbol_kind::S_ICONST: // ICONST
               value.template destroy<int>();
               break;

            case symbol_kind::S_when_clause: // when_clause
               value.template destroy<lingodb::ast::CaseExpression::CaseCheck>();
               break;

            case symbol_kind::S_basicComparisonType: // basicComparisonType
               value.template destroy<lingodb::ast::ExpressionType>();
               break;

            case symbol_kind::S_join_type: // join_type
               value.template destroy<lingodb::ast::JoinType>();
               break;

            case symbol_kind::S_Type: // Type
            case symbol_kind::S_SimpleType: // SimpleType
            case symbol_kind::S_ConstTypename: // ConstTypename
            case symbol_kind::S_Numeric_with_opt_lenghth: // Numeric_with_opt_lenghth
            case symbol_kind::S_Numeric: // Numeric
            case symbol_kind::S_Character: // Character
            case symbol_kind::S_ConstCharacter: // ConstCharacter
            case symbol_kind::S_CharacterWithLength: // CharacterWithLength
            case symbol_kind::S_CharacterWithoutLength: // CharacterWithoutLength
            case symbol_kind::S_character: // character
            case symbol_kind::S_ConstDatetime: // ConstDatetime
            case symbol_kind::S_ConstInterval: // ConstInterval
            case symbol_kind::S_Typename: // Typename
               value.template destroy<lingodb::ast::LogicalTypeWithMods>();
               break;

            case symbol_kind::S_opt_nulls_order: // opt_nulls_order
               value.template destroy<lingodb::ast::OrderByNullType>();
               break;

            case symbol_kind::S_opt_asc_desc: // opt_asc_desc
               value.template destroy<lingodb::ast::OrderType>();
               break;

            case symbol_kind::S_sub_type: // sub_type
               value.template destroy<lingodb::ast::SubqueryType>();
               break;

            case symbol_kind::S_join_qual: // join_qual
               value.template destroy<lingodb::ast::jointCondOrUsingCols>();
               break;

            case symbol_kind::S_opt_interval: // opt_interval
               value.template destroy<std::optional<lingodb::ast::SQLAbstractLogicalType>>();
               break;

            case symbol_kind::S_opt_sort_clause: // opt_sort_clause
               value.template destroy<std::optional<std::shared_ptr<lingodb::ast::OrderByModifier>>>();
               break;

            case symbol_kind::S_case_arg: // case_arg
               value.template destroy<std::optional<std::shared_ptr<lingodb::ast::ParsedExpression>>>();
               break;

            case symbol_kind::S_opt_select_limit: // opt_select_limit
               value.template destroy<std::optional<std::shared_ptr<lingodb::ast::ResultModifier>>>();
               break;

            case symbol_kind::S_copy_opt_item: // copy_opt_item
               value.template destroy<std::pair<std::string, std::string>>();
               break;

            case symbol_kind::S_alias_clause: // alias_clause
            case symbol_kind::S_opt_alias_clause: // opt_alias_clause
               value.template destroy<std::pair<std::string, std::vector<std::string>>>();
               break;

            case symbol_kind::S_extract_arg: // extract_arg
               value.template destroy<std::shared_ptr<ast::ConstantExpression>>();
               break;

            case symbol_kind::S_CopyStmt: // CopyStmt
               value.template destroy<std::shared_ptr<ast::CopyNode>>();
               break;

            case symbol_kind::S_VariableSetStmt: // VariableSetStmt
            case symbol_kind::S_set_rest: // set_rest
            case symbol_kind::S_generic_set: // generic_set
            case symbol_kind::S_set_rest_more: // set_rest_more
               value.template destroy<std::shared_ptr<ast::SetVariableStatement>>();
               break;

            case symbol_kind::S_agg_expr: // agg_expr
               value.template destroy<std::shared_ptr<lingodb::ast::AggregationNode>>();
               break;

            case symbol_kind::S_toplevel_stmt: // toplevel_stmt
            case symbol_kind::S_stmt: // stmt
               value.template destroy<std::shared_ptr<lingodb::ast::AstNode>>();
               break;

            case symbol_kind::S_and_a_expr: // and_a_expr
            case symbol_kind::S_or_a_expr: // or_a_expr
               value.template destroy<std::shared_ptr<lingodb::ast::ConjunctionExpression>>();
               break;

            case symbol_kind::S_ColConstraint: // ColConstraint
            case symbol_kind::S_ColConstraintElem: // ColConstraintElem
            case symbol_kind::S_ConstraintElem: // ConstraintElem
               value.template destroy<std::shared_ptr<lingodb::ast::Constraint>>();
               break;

            case symbol_kind::S_CreateStmt: // CreateStmt
               value.template destroy<std::shared_ptr<lingodb::ast::CreateNode>>();
               break;

            case symbol_kind::S_from_list: // from_list
               value.template destroy<std::shared_ptr<lingodb::ast::CrossProductRef>>();
               break;

            case symbol_kind::S_values_clause: // values_clause
               value.template destroy<std::shared_ptr<lingodb::ast::ExpressionListRef>>();
               break;

            case symbol_kind::S_func_application: // func_application
            case symbol_kind::S_func_expr: // func_expr
            case symbol_kind::S_func_expr_common_subexpr: // func_expr_common_subexpr
               value.template destroy<std::shared_ptr<lingodb::ast::FunctionExpression>>();
               break;

            case symbol_kind::S_group_clause: // group_clause
            case symbol_kind::S_group_clause_with_alias: // group_clause_with_alias
               value.template destroy<std::shared_ptr<lingodb::ast::GroupByNode>>();
               break;

            case symbol_kind::S_InsertStmt: // InsertStmt
            case symbol_kind::S_insert_rest: // insert_rest
               value.template destroy<std::shared_ptr<lingodb::ast::InsertNode>>();
               break;

            case symbol_kind::S_select_limit: // select_limit
            case symbol_kind::S_limit_clause: // limit_clause
               value.template destroy<std::shared_ptr<lingodb::ast::LimitModifier>>();
               break;

            case symbol_kind::S_sortby: // sortby
               value.template destroy<std::shared_ptr<lingodb::ast::OrderByElement>>();
               break;

            case symbol_kind::S_sort_clause: // sort_clause
               value.template destroy<std::shared_ptr<lingodb::ast::OrderByModifier>>();
               break;

            case symbol_kind::S_var_value: // var_value
            case symbol_kind::S_opt_boolean_or_string: // opt_boolean_or_string
            case symbol_kind::S_NonReservedWord_or_Sconst: // NonReservedWord_or_Sconst
            case symbol_kind::S_group_by_item: // group_by_item
            case symbol_kind::S_group_by_item_with_alias: // group_by_item_with_alias
            case symbol_kind::S_having_clause: // having_clause
            case symbol_kind::S_offset_clause: // offset_clause
            case symbol_kind::S_select_limit_value: // select_limit_value
            case symbol_kind::S_select_offset_value: // select_offset_value
            case symbol_kind::S_where_clause: // where_clause
            case symbol_kind::S_a_expr: // a_expr
            case symbol_kind::S_b_expr: // b_expr
            case symbol_kind::S_c_expr: // c_expr
            case symbol_kind::S_case_expr: // case_expr
            case symbol_kind::S_case_default: // case_default
            case symbol_kind::S_columnref: // columnref
            case symbol_kind::S_func_arg_expr: // func_arg_expr
            case symbol_kind::S_cast_expr: // cast_expr
            case symbol_kind::S_indirection: // indirection
            case symbol_kind::S_indirection_el: // indirection_el
            case symbol_kind::S_target_el: // target_el
            case symbol_kind::S_NumericOnly: // NumericOnly
            case symbol_kind::S_AexprConst: // AexprConst
            case symbol_kind::S_Iconst: // Iconst
            case symbol_kind::S_SignedIconst: // SignedIconst
            case symbol_kind::S_Fconst: // Fconst
            case symbol_kind::S_Sconst: // Sconst
            case symbol_kind::S_Bconst: // Bconst
               value.template destroy<std::shared_ptr<lingodb::ast::ParsedExpression>>();
               break;

            case symbol_kind::S_pipe_start: // pipe_start
            case symbol_kind::S_pipe_operator: // pipe_operator
               value.template destroy<std::shared_ptr<lingodb::ast::PipeOperator>>();
               break;

            case symbol_kind::S_select_with_parens: // select_with_parens
            case symbol_kind::S_select_no_parens: // select_no_parens
            case symbol_kind::S_select_clause: // select_clause
            case symbol_kind::S_simple_select: // simple_select
            case symbol_kind::S_with_clause: // with_clause
            case symbol_kind::S_cte_list: // cte_list
            case symbol_kind::S_common_table_expr: // common_table_expr
               value.template destroy<std::shared_ptr<lingodb::ast::QueryNode>>();
               break;

            case symbol_kind::S_set_list: // set_list
               value.template destroy<std::shared_ptr<lingodb::ast::SetColumnExpression>>();
               break;

            case symbol_kind::S_TableElement: // TableElement
            case symbol_kind::S_TableConstraint: // TableConstraint
            case symbol_kind::S_columnElement: // columnElement
               value.template destroy<std::shared_ptr<lingodb::ast::TableElement>>();
               break;

            case symbol_kind::S_SelectStmt: // SelectStmt
            case symbol_kind::S_PipeSQLStmt: // PipeSQLStmt
            case symbol_kind::S_classic_select_and_pipe_sql_with_parens: // classic_select_and_pipe_sql_with_parens
            case symbol_kind::S_pipe_sql_with_parens: // pipe_sql_with_parens
            case symbol_kind::S_pipe_sql_no_parens: // pipe_sql_no_parens
            case symbol_kind::S_pipe_or_select_clause: // pipe_or_select_clause
            case symbol_kind::S_PreparableStmt: // PreparableStmt
               value.template destroy<std::shared_ptr<lingodb::ast::TableProducer>>();
               break;

            case symbol_kind::S_opt_from_clause: // opt_from_clause
            case symbol_kind::S_from_clause: // from_clause
            case symbol_kind::S_table_ref: // table_ref
            case symbol_kind::S_joined_table: // joined_table
               value.template destroy<std::shared_ptr<lingodb::ast::TableRef>>();
               break;

            case symbol_kind::S_opt_target_list: // opt_target_list
               value.template destroy<std::shared_ptr<lingodb::ast::TargetList>>();
               break;

            case symbol_kind::S_type_modifier: // type_modifier
               value.template destroy<std::shared_ptr<lingodb::ast::Value>>();
               break;

            case symbol_kind::S_window_func_expr: // window_func_expr
            case symbol_kind::S_over_clause: // over_clause
            case symbol_kind::S_window_specification: // window_specification
               value.template destroy<std::shared_ptr<lingodb::ast::WindowExpression>>();
               break;

            case symbol_kind::S_opt_frame_clause: // opt_frame_clause
            case symbol_kind::S_frame_extent: // frame_extent
            case symbol_kind::S_frame_bound: // frame_bound
               value.template destroy<std::shared_ptr<lingodb::ast::WindowFrame>>();
               break;

            case symbol_kind::S_FCONST: // FCONST
            case symbol_kind::S_IDENT: // IDENT
            case symbol_kind::S_IDENTIFIER: // "identifier"
            case symbol_kind::S_STRING_VALUE: // STRING_VALUE
            case symbol_kind::S_BIT_VALUE: // "bit_string"
            case symbol_kind::S_HEX_VALUE: // "hex_string"
            case symbol_kind::S_NATIONAL_VALUE: // "nat_string"
            case symbol_kind::S_PERCENT: // "%"
            case symbol_kind::S_PLUS: // "+"
            case symbol_kind::S_SLASH: // "/"
            case symbol_kind::S_STAR: // "*"
            case symbol_kind::S_MINUS: // "-"
            case symbol_kind::S_EQUAL: // "="
            case symbol_kind::S_NOT_EQUAL: // "<>"
            case symbol_kind::S_LESS_EQUAL: // "<="
            case symbol_kind::S_LESS: // "<"
            case symbol_kind::S_GREATER_EQUAL: // ">="
            case symbol_kind::S_GREATER: // ">"
            case symbol_kind::S_UIDENT: // UIDENT
            case symbol_kind::S_SCONST: // SCONST
            case symbol_kind::S_USCONST: // USCONST
            case symbol_kind::S_BCONST: // BCONST
            case symbol_kind::S_XCONST: // XCONST
            case symbol_kind::S_Op: // Op
            case symbol_kind::S_TYPECAST: // TYPECAST
            case symbol_kind::S_ABORT_P: // ABORT_P
            case symbol_kind::S_ABSENT: // ABSENT
            case symbol_kind::S_ABSOLUTE_P: // ABSOLUTE_P
            case symbol_kind::S_ACCESS: // ACCESS
            case symbol_kind::S_ACTION: // ACTION
            case symbol_kind::S_ADD_P: // ADD_P
            case symbol_kind::S_ADMIN: // ADMIN
            case symbol_kind::S_AFTER: // AFTER
            case symbol_kind::S_AGGREGATE: // AGGREGATE
            case symbol_kind::S_ALL: // ALL
            case symbol_kind::S_ALSO: // ALSO
            case symbol_kind::S_ALTER: // ALTER
            case symbol_kind::S_ALWAYS: // ALWAYS
            case symbol_kind::S_ANALYSE: // ANALYSE
            case symbol_kind::S_ANALYZE: // ANALYZE
            case symbol_kind::S_AND: // AND
            case symbol_kind::S_ANY: // ANY
            case symbol_kind::S_ARRAY: // ARRAY
            case symbol_kind::S_AS: // AS
            case symbol_kind::S_ASC: // ASC
            case symbol_kind::S_ASENSITIVE: // ASENSITIVE
            case symbol_kind::S_ASSERTION: // ASSERTION
            case symbol_kind::S_ASSIGNMENT: // ASSIGNMENT
            case symbol_kind::S_ASYMMETRIC: // ASYMMETRIC
            case symbol_kind::S_ATOMIC: // ATOMIC
            case symbol_kind::S_AT: // AT
            case symbol_kind::S_ATTACH: // ATTACH
            case symbol_kind::S_ATTRIBUTE: // ATTRIBUTE
            case symbol_kind::S_AUTHORIZATION: // AUTHORIZATION
            case symbol_kind::S_BACKWARD: // BACKWARD
            case symbol_kind::S_BEFORE: // BEFORE
            case symbol_kind::S_BEGIN_P: // BEGIN_P
            case symbol_kind::S_BETWEEN: // BETWEEN
            case symbol_kind::S_BIGINT: // BIGINT
            case symbol_kind::S_BINARY: // BINARY
            case symbol_kind::S_BIT: // BIT
            case symbol_kind::S_BOOLEAN_P: // BOOLEAN_P
            case symbol_kind::S_BOTH: // BOTH
            case symbol_kind::S_BREADTH: // BREADTH
            case symbol_kind::S_BY: // BY
            case symbol_kind::S_CACHE: // CACHE
            case symbol_kind::S_CALL: // CALL
            case symbol_kind::S_CALLED: // CALLED
            case symbol_kind::S_CASCADE: // CASCADE
            case symbol_kind::S_CASCADED: // CASCADED
            case symbol_kind::S_CASE: // CASE
            case symbol_kind::S_CAST: // CAST
            case symbol_kind::S_CATALOG_P: // CATALOG_P
            case symbol_kind::S_CHAIN: // CHAIN
            case symbol_kind::S_CHAR_P: // CHAR_P
            case symbol_kind::S_CHARACTER: // CHARACTER
            case symbol_kind::S_CHARACTERISTICS: // CHARACTERISTICS
            case symbol_kind::S_CHECK: // CHECK
            case symbol_kind::S_CHECKPOINT: // CHECKPOINT
            case symbol_kind::S_CLASS: // CLASS
            case symbol_kind::S_CLOSE: // CLOSE
            case symbol_kind::S_CLUSTER: // CLUSTER
            case symbol_kind::S_COALESCE: // COALESCE
            case symbol_kind::S_COLLATE: // COLLATE
            case symbol_kind::S_COLLATION: // COLLATION
            case symbol_kind::S_COLUMN: // COLUMN
            case symbol_kind::S_COLUMNS: // COLUMNS
            case symbol_kind::S_COMMENT: // COMMENT
            case symbol_kind::S_COMMENTS: // COMMENTS
            case symbol_kind::S_COMMIT: // COMMIT
            case symbol_kind::S_COMMITTED: // COMMITTED
            case symbol_kind::S_COMPRESSION: // COMPRESSION
            case symbol_kind::S_CONCURRENTLY: // CONCURRENTLY
            case symbol_kind::S_CONDITIONAL: // CONDITIONAL
            case symbol_kind::S_CONFIGURATION: // CONFIGURATION
            case symbol_kind::S_CONFLICT: // CONFLICT
            case symbol_kind::S_CONNECTION: // CONNECTION
            case symbol_kind::S_CONSTRAINT: // CONSTRAINT
            case symbol_kind::S_CONSTRAINTS: // CONSTRAINTS
            case symbol_kind::S_CONTENT_P: // CONTENT_P
            case symbol_kind::S_CONTINUE_P: // CONTINUE_P
            case symbol_kind::S_CONVERSION_P: // CONVERSION_P
            case symbol_kind::S_COPY: // COPY
            case symbol_kind::S_COST: // COST
            case symbol_kind::S_CREATE: // CREATE
            case symbol_kind::S_CROSS: // CROSS
            case symbol_kind::S_CSV: // CSV
            case symbol_kind::S_CUBE: // CUBE
            case symbol_kind::S_CURRENT_P: // CURRENT_P
            case symbol_kind::S_CURRENT_CATALOG: // CURRENT_CATALOG
            case symbol_kind::S_CURRENT_DATE: // CURRENT_DATE
            case symbol_kind::S_CURRENT_ROLE: // CURRENT_ROLE
            case symbol_kind::S_CURRENT_SCHEMA: // CURRENT_SCHEMA
            case symbol_kind::S_CURRENT_TIME: // CURRENT_TIME
            case symbol_kind::S_CURRENT_TIMESTAMP: // CURRENT_TIMESTAMP
            case symbol_kind::S_CURRENT_USER: // CURRENT_USER
            case symbol_kind::S_CURSOR: // CURSOR
            case symbol_kind::S_CYCLE: // CYCLE
            case symbol_kind::S_DATA_P: // DATA_P
            case symbol_kind::S_DATABASE: // DATABASE
            case symbol_kind::S_DAY_P: // DAY_P
            case symbol_kind::S_DEALLOCATE: // DEALLOCATE
            case symbol_kind::S_DEC: // DEC
            case symbol_kind::S_DECIMAL_P: // DECIMAL_P
            case symbol_kind::S_DECLARE: // DECLARE
            case symbol_kind::S_DEFAULT: // DEFAULT
            case symbol_kind::S_DEFAULTS: // DEFAULTS
            case symbol_kind::S_DATE_P: // DATE_P
            case symbol_kind::S_DEFERRABLE: // DEFERRABLE
            case symbol_kind::S_DEFERRED: // DEFERRED
            case symbol_kind::S_DEFINER: // DEFINER
            case symbol_kind::S_DELETE_P: // DELETE_P
            case symbol_kind::S_DELIMITER: // DELIMITER
            case symbol_kind::S_DELIMITERS: // DELIMITERS
            case symbol_kind::S_DEPENDS: // DEPENDS
            case symbol_kind::S_DEPTH: // DEPTH
            case symbol_kind::S_DESC: // DESC
            case symbol_kind::S_DETACH: // DETACH
            case symbol_kind::S_DICTIONARY: // DICTIONARY
            case symbol_kind::S_DISABLE_P: // DISABLE_P
            case symbol_kind::S_DISCARD: // DISCARD
            case symbol_kind::S_DISTINCT: // DISTINCT
            case symbol_kind::S_DO: // DO
            case symbol_kind::S_DOCUMENT_P: // DOCUMENT_P
            case symbol_kind::S_DOMAIN_P: // DOMAIN_P
            case symbol_kind::S_DOUBLE_P: // DOUBLE_P
            case symbol_kind::S_DROP: // DROP
            case symbol_kind::S_EACH: // EACH
            case symbol_kind::S_ELSE: // ELSE
            case symbol_kind::S_EMPTY_P: // EMPTY_P
            case symbol_kind::S_ENABLE_P: // ENABLE_P
            case symbol_kind::S_ENCODING: // ENCODING
            case symbol_kind::S_ENCRYPTED: // ENCRYPTED
            case symbol_kind::S_END_P: // END_P
            case symbol_kind::S_ENFORCED: // ENFORCED
            case symbol_kind::S_ENUM_P: // ENUM_P
            case symbol_kind::S_ERROR_P: // ERROR_P
            case symbol_kind::S_ESCAPE: // ESCAPE
            case symbol_kind::S_EVENT: // EVENT
            case symbol_kind::S_EXCEPT: // EXCEPT
            case symbol_kind::S_EXCLUDE: // EXCLUDE
            case symbol_kind::S_EXCLUDING: // EXCLUDING
            case symbol_kind::S_EXCLUSIVE: // EXCLUSIVE
            case symbol_kind::S_EXECUTE: // EXECUTE
            case symbol_kind::S_EXISTS: // EXISTS
            case symbol_kind::S_EXPLAIN: // EXPLAIN
            case symbol_kind::S_EXPRESSION: // EXPRESSION
            case symbol_kind::S_EXTENSION: // EXTENSION
            case symbol_kind::S_EXTERNAL: // EXTERNAL
            case symbol_kind::S_EXTRACT: // EXTRACT
            case symbol_kind::S_FALSE_P: // FALSE_P
            case symbol_kind::S_FAMILY: // FAMILY
            case symbol_kind::S_FETCH: // FETCH
            case symbol_kind::S_FILTER: // FILTER
            case symbol_kind::S_FINALIZE: // FINALIZE
            case symbol_kind::S_FIRST_P: // FIRST_P
            case symbol_kind::S_FLOAT_P: // FLOAT_P
            case symbol_kind::S_FOLLOWING: // FOLLOWING
            case symbol_kind::S_FOR: // FOR
            case symbol_kind::S_FORCE: // FORCE
            case symbol_kind::S_FOREIGN: // FOREIGN
            case symbol_kind::S_FORMAT: // FORMAT
            case symbol_kind::S_FORWARD: // FORWARD
            case symbol_kind::S_FREEZE: // FREEZE
            case symbol_kind::S_FROM: // FROM
            case symbol_kind::S_FULL: // FULL
            case symbol_kind::S_FUNCTION: // FUNCTION
            case symbol_kind::S_FUNCTIONS: // FUNCTIONS
            case symbol_kind::S_GENERATED: // GENERATED
            case symbol_kind::S_GLOBAL: // GLOBAL
            case symbol_kind::S_GRANT: // GRANT
            case symbol_kind::S_GRANTED: // GRANTED
            case symbol_kind::S_GREATEST: // GREATEST
            case symbol_kind::S_GROUP_P: // GROUP_P
            case symbol_kind::S_GROUPING: // GROUPING
            case symbol_kind::S_GROUPS: // GROUPS
            case symbol_kind::S_HANDLER: // HANDLER
            case symbol_kind::S_HAVING: // HAVING
            case symbol_kind::S_HEADER_P: // HEADER_P
            case symbol_kind::S_HOLD: // HOLD
            case symbol_kind::S_HOUR_P: // HOUR_P
            case symbol_kind::S_IDENTITY_P: // IDENTITY_P
            case symbol_kind::S_IF_P: // IF_P
            case symbol_kind::S_ILIKE: // ILIKE
            case symbol_kind::S_IMMEDIATE: // IMMEDIATE
            case symbol_kind::S_IMMUTABLE: // IMMUTABLE
            case symbol_kind::S_IMPLICIT_P: // IMPLICIT_P
            case symbol_kind::S_IMPORT_P: // IMPORT_P
            case symbol_kind::S_IN_P: // IN_P
            case symbol_kind::S_INCLUDE: // INCLUDE
            case symbol_kind::S_INCLUDING: // INCLUDING
            case symbol_kind::S_INCREMENT: // INCREMENT
            case symbol_kind::S_INDENT: // INDENT
            case symbol_kind::S_INDEX: // INDEX
            case symbol_kind::S_INDEXES: // INDEXES
            case symbol_kind::S_INHERIT: // INHERIT
            case symbol_kind::S_INHERITS: // INHERITS
            case symbol_kind::S_INITIALLY: // INITIALLY
            case symbol_kind::S_INLINE_P: // INLINE_P
            case symbol_kind::S_INNER_P: // INNER_P
            case symbol_kind::S_INOUT: // INOUT
            case symbol_kind::S_INPUT_P: // INPUT_P
            case symbol_kind::S_INSENSITIVE: // INSENSITIVE
            case symbol_kind::S_INSERT: // INSERT
            case symbol_kind::S_INSTEAD: // INSTEAD
            case symbol_kind::S_INT_P: // INT_P
            case symbol_kind::S_INTEGER: // INTEGER
            case symbol_kind::S_INTERSECT: // INTERSECT
            case symbol_kind::S_INTERVAL: // INTERVAL
            case symbol_kind::S_INTO: // INTO
            case symbol_kind::S_INVOKER: // INVOKER
            case symbol_kind::S_IS: // IS
            case symbol_kind::S_ISNULL: // ISNULL
            case symbol_kind::S_ISOLATION: // ISOLATION
            case symbol_kind::S_JOIN: // JOIN
            case symbol_kind::S_JSON: // JSON
            case symbol_kind::S_JSON_ARRAY: // JSON_ARRAY
            case symbol_kind::S_JSON_ARRAYAGG: // JSON_ARRAYAGG
            case symbol_kind::S_JSON_EXISTS: // JSON_EXISTS
            case symbol_kind::S_JSON_OBJECT: // JSON_OBJECT
            case symbol_kind::S_JSON_OBJECTAGG: // JSON_OBJECTAGG
            case symbol_kind::S_JSON_QUERY: // JSON_QUERY
            case symbol_kind::S_JSON_SCALAR: // JSON_SCALAR
            case symbol_kind::S_JSON_SERIALIZE: // JSON_SERIALIZE
            case symbol_kind::S_JSON_TABLE: // JSON_TABLE
            case symbol_kind::S_JSON_VALUE: // JSON_VALUE
            case symbol_kind::S_KEEP: // KEEP
            case symbol_kind::S_KEY: // KEY
            case symbol_kind::S_KEYS: // KEYS
            case symbol_kind::S_LABEL: // LABEL
            case symbol_kind::S_LANGUAGE: // LANGUAGE
            case symbol_kind::S_LARGE_P: // LARGE_P
            case symbol_kind::S_LAST_P: // LAST_P
            case symbol_kind::S_LATERAL_P: // LATERAL_P
            case symbol_kind::S_LEADING: // LEADING
            case symbol_kind::S_LEAKPROOF: // LEAKPROOF
            case symbol_kind::S_LEAST: // LEAST
            case symbol_kind::S_LEFT: // LEFT
            case symbol_kind::S_LEVEL: // LEVEL
            case symbol_kind::S_LIKE: // LIKE
            case symbol_kind::S_LIMIT: // LIMIT
            case symbol_kind::S_LISTEN: // LISTEN
            case symbol_kind::S_LOAD: // LOAD
            case symbol_kind::S_LOCAL: // LOCAL
            case symbol_kind::S_LOCALTIME: // LOCALTIME
            case symbol_kind::S_LOCALTIMESTAMP: // LOCALTIMESTAMP
            case symbol_kind::S_LOCATION: // LOCATION
            case symbol_kind::S_LOCK_P: // LOCK_P
            case symbol_kind::S_LOCKED: // LOCKED
            case symbol_kind::S_LOGGED: // LOGGED
            case symbol_kind::S_MAPPING: // MAPPING
            case symbol_kind::S_MATCH: // MATCH
            case symbol_kind::S_MATCHED: // MATCHED
            case symbol_kind::S_MATERIALIZED: // MATERIALIZED
            case symbol_kind::S_MAXVALUE: // MAXVALUE
            case symbol_kind::S_MERGE: // MERGE
            case symbol_kind::S_MERGE_ACTION: // MERGE_ACTION
            case symbol_kind::S_METHOD: // METHOD
            case symbol_kind::S_MINUTE_P: // MINUTE_P
            case symbol_kind::S_MINVALUE: // MINVALUE
            case symbol_kind::S_MODE: // MODE
            case symbol_kind::S_MONTH_P: // MONTH_P
            case symbol_kind::S_MOVE: // MOVE
            case symbol_kind::S_NAME_P: // NAME_P
            case symbol_kind::S_NAMES: // NAMES
            case symbol_kind::S_NATIONAL: // NATIONAL
            case symbol_kind::S_NATURAL: // NATURAL
            case symbol_kind::S_NCHAR: // NCHAR
            case symbol_kind::S_NESTED: // NESTED
            case symbol_kind::S_NEW: // NEW
            case symbol_kind::S_NEXT: // NEXT
            case symbol_kind::S_NFC: // NFC
            case symbol_kind::S_NFD: // NFD
            case symbol_kind::S_NFKC: // NFKC
            case symbol_kind::S_NFKD: // NFKD
            case symbol_kind::S_NO: // NO
            case symbol_kind::S_NONE: // NONE
            case symbol_kind::S_NORMALIZE: // NORMALIZE
            case symbol_kind::S_NORMALIZED: // NORMALIZED
            case symbol_kind::S_NOT: // NOT
            case symbol_kind::S_NOTHING: // NOTHING
            case symbol_kind::S_NOTIFY: // NOTIFY
            case symbol_kind::S_NOTNULL: // NOTNULL
            case symbol_kind::S_NOWAIT: // NOWAIT
            case symbol_kind::S_NULL_P: // NULL_P
            case symbol_kind::S_NULLIF: // NULLIF
            case symbol_kind::S_NULLS_P: // NULLS_P
            case symbol_kind::S_NUMERIC: // NUMERIC
            case symbol_kind::S_OBJECT_P: // OBJECT_P
            case symbol_kind::S_OBJECTS_P: // OBJECTS_P
            case symbol_kind::S_OF: // OF
            case symbol_kind::S_OFF: // OFF
            case symbol_kind::S_OFFSET: // OFFSET
            case symbol_kind::S_OIDS: // OIDS
            case symbol_kind::S_OLD: // OLD
            case symbol_kind::S_OMIT: // OMIT
            case symbol_kind::S_ON: // ON
            case symbol_kind::S_ONLY: // ONLY
            case symbol_kind::S_OPERATOR: // OPERATOR
            case symbol_kind::S_OPTION: // OPTION
            case symbol_kind::S_OPTIONS: // OPTIONS
            case symbol_kind::S_OR: // OR
            case symbol_kind::S_ORDER: // ORDER
            case symbol_kind::S_ORDINALITY: // ORDINALITY
            case symbol_kind::S_OTHERS: // OTHERS
            case symbol_kind::S_OUT_P: // OUT_P
            case symbol_kind::S_OUTER_P: // OUTER_P
            case symbol_kind::S_OVER: // OVER
            case symbol_kind::S_OVERLAPS: // OVERLAPS
            case symbol_kind::S_OVERLAY: // OVERLAY
            case symbol_kind::S_OVERRIDING: // OVERRIDING
            case symbol_kind::S_OWNED: // OWNED
            case symbol_kind::S_OWNER: // OWNER
            case symbol_kind::S_PARALLEL: // PARALLEL
            case symbol_kind::S_PARAMETER: // PARAMETER
            case symbol_kind::S_PARSER: // PARSER
            case symbol_kind::S_PARTIAL: // PARTIAL
            case symbol_kind::S_PARTITION: // PARTITION
            case symbol_kind::S_PASSING: // PASSING
            case symbol_kind::S_PASSWORD: // PASSWORD
            case symbol_kind::S_PATH: // PATH
            case symbol_kind::S_PERIOD: // PERIOD
            case symbol_kind::S_PLACING: // PLACING
            case symbol_kind::S_PLAN: // PLAN
            case symbol_kind::S_PLANS: // PLANS
            case symbol_kind::S_POLICY: // POLICY
            case symbol_kind::S_POSITION: // POSITION
            case symbol_kind::S_PRECEDING: // PRECEDING
            case symbol_kind::S_PRECISION: // PRECISION
            case symbol_kind::S_PRESERVE: // PRESERVE
            case symbol_kind::S_PREPARE: // PREPARE
            case symbol_kind::S_PREPARED: // PREPARED
            case symbol_kind::S_PRIMARY: // PRIMARY
            case symbol_kind::S_PRIOR: // PRIOR
            case symbol_kind::S_PRIVILEGES: // PRIVILEGES
            case symbol_kind::S_PROCEDURAL: // PROCEDURAL
            case symbol_kind::S_PROCEDURE: // PROCEDURE
            case symbol_kind::S_PROCEDURES: // PROCEDURES
            case symbol_kind::S_PROGRAM: // PROGRAM
            case symbol_kind::S_PUBLICATION: // PUBLICATION
            case symbol_kind::S_QUOTES: // QUOTES
            case symbol_kind::S_EXTEND: // EXTEND
            case symbol_kind::S_RANGE: // RANGE
            case symbol_kind::S_READ: // READ
            case symbol_kind::S_REAL: // REAL
            case symbol_kind::S_REASSIGN: // REASSIGN
            case symbol_kind::S_RECURSIVE: // RECURSIVE
            case symbol_kind::S_REF_P: // REF_P
            case symbol_kind::S_REFERENCES: // REFERENCES
            case symbol_kind::S_REFERENCING: // REFERENCING
            case symbol_kind::S_REFRESH: // REFRESH
            case symbol_kind::S_REINDEX: // REINDEX
            case symbol_kind::S_RELATIVE_P: // RELATIVE_P
            case symbol_kind::S_RELEASE: // RELEASE
            case symbol_kind::S_RENAME: // RENAME
            case symbol_kind::S_REPEATABLE: // REPEATABLE
            case symbol_kind::S_REPLACE: // REPLACE
            case symbol_kind::S_REPLICA: // REPLICA
            case symbol_kind::S_RESET: // RESET
            case symbol_kind::S_RESTART: // RESTART
            case symbol_kind::S_RESTRICT: // RESTRICT
            case symbol_kind::S_RETURN: // RETURN
            case symbol_kind::S_RETURNING: // RETURNING
            case symbol_kind::S_RETURNS: // RETURNS
            case symbol_kind::S_REVOKE: // REVOKE
            case symbol_kind::S_RIGHT: // RIGHT
            case symbol_kind::S_ROLE: // ROLE
            case symbol_kind::S_ROLLBACK: // ROLLBACK
            case symbol_kind::S_ROLLUP: // ROLLUP
            case symbol_kind::S_ROUTINE: // ROUTINE
            case symbol_kind::S_ROUTINES: // ROUTINES
            case symbol_kind::S_ROW: // ROW
            case symbol_kind::S_ROWS: // ROWS
            case symbol_kind::S_RULE: // RULE
            case symbol_kind::S_SAVEPOINT: // SAVEPOINT
            case symbol_kind::S_SCALAR: // SCALAR
            case symbol_kind::S_SCHEMA: // SCHEMA
            case symbol_kind::S_SCHEMAS: // SCHEMAS
            case symbol_kind::S_SCROLL: // SCROLL
            case symbol_kind::S_SEARCH: // SEARCH
            case symbol_kind::S_SECOND_P: // SECOND_P
            case symbol_kind::S_SECURITY: // SECURITY
            case symbol_kind::S_SELECT: // SELECT
            case symbol_kind::S_SEQUENCE: // SEQUENCE
            case symbol_kind::S_SEQUENCES: // SEQUENCES
            case symbol_kind::S_SERIALIZABLE: // SERIALIZABLE
            case symbol_kind::S_SERVER: // SERVER
            case symbol_kind::S_SESSION: // SESSION
            case symbol_kind::S_SESSION_USER: // SESSION_USER
            case symbol_kind::S_SET: // SET
            case symbol_kind::S_SETS: // SETS
            case symbol_kind::S_SETOF: // SETOF
            case symbol_kind::S_SHARE: // SHARE
            case symbol_kind::S_SHOW: // SHOW
            case symbol_kind::S_SIMILAR: // SIMILAR
            case symbol_kind::S_SIMPLE: // SIMPLE
            case symbol_kind::S_SKIP: // SKIP
            case symbol_kind::S_SMALLINT: // SMALLINT
            case symbol_kind::S_SNAPSHOT: // SNAPSHOT
            case symbol_kind::S_SOME: // SOME
            case symbol_kind::S_SOURCE: // SOURCE
            case symbol_kind::S_SQL_P: // SQL_P
            case symbol_kind::S_STABLE: // STABLE
            case symbol_kind::S_STANDALONE_P: // STANDALONE_P
            case symbol_kind::S_START: // START
            case symbol_kind::S_STATEMENT: // STATEMENT
            case symbol_kind::S_STATISTICS: // STATISTICS
            case symbol_kind::S_STDIN: // STDIN
            case symbol_kind::S_STDOUT: // STDOUT
            case symbol_kind::S_STORAGE: // STORAGE
            case symbol_kind::S_STORED: // STORED
            case symbol_kind::S_STRICT_P: // STRICT_P
            case symbol_kind::S_STRING_P: // STRING_P
            case symbol_kind::S_STRIP_P: // STRIP_P
            case symbol_kind::S_SUBSCRIPTION: // SUBSCRIPTION
            case symbol_kind::S_SUBSTRING: // SUBSTRING
            case symbol_kind::S_SUPPORT: // SUPPORT
            case symbol_kind::S_SYMMETRIC: // SYMMETRIC
            case symbol_kind::S_SYSID: // SYSID
            case symbol_kind::S_SYSTEM_P: // SYSTEM_P
            case symbol_kind::S_SYSTEM_USER: // SYSTEM_USER
            case symbol_kind::S_TABLE: // TABLE
            case symbol_kind::S_TABLES: // TABLES
            case symbol_kind::S_TABLESAMPLE: // TABLESAMPLE
            case symbol_kind::S_TABLESPACE: // TABLESPACE
            case symbol_kind::S_TARGET: // TARGET
            case symbol_kind::S_TEMP: // TEMP
            case symbol_kind::S_TEMPLATE: // TEMPLATE
            case symbol_kind::S_TEMPORARY: // TEMPORARY
            case symbol_kind::S_TEXT_P: // TEXT_P
            case symbol_kind::S_THEN: // THEN
            case symbol_kind::S_TIES: // TIES
            case symbol_kind::S_TIME: // TIME
            case symbol_kind::S_TIMESTAMP: // TIMESTAMP
            case symbol_kind::S_TO: // TO
            case symbol_kind::S_TRAILING: // TRAILING
            case symbol_kind::S_TRANSACTION: // TRANSACTION
            case symbol_kind::S_TRANSFORM: // TRANSFORM
            case symbol_kind::S_TREAT: // TREAT
            case symbol_kind::S_TRIGGER: // TRIGGER
            case symbol_kind::S_TRIM: // TRIM
            case symbol_kind::S_TRUE_P: // TRUE_P
            case symbol_kind::S_TRUNCATE: // TRUNCATE
            case symbol_kind::S_TRUSTED: // TRUSTED
            case symbol_kind::S_TYPE_P: // TYPE_P
            case symbol_kind::S_TYPES_P: // TYPES_P
            case symbol_kind::S_UESCAPE: // UESCAPE
            case symbol_kind::S_UNBOUNDED: // UNBOUNDED
            case symbol_kind::S_UNCONDITIONAL: // UNCONDITIONAL
            case symbol_kind::S_UNCOMMITTED: // UNCOMMITTED
            case symbol_kind::S_UNENCRYPTED: // UNENCRYPTED
            case symbol_kind::S_UNION: // UNION
            case symbol_kind::S_UNIQUE: // UNIQUE
            case symbol_kind::S_UNKNOWN: // UNKNOWN
            case symbol_kind::S_UNLISTEN: // UNLISTEN
            case symbol_kind::S_UNLOGGED: // UNLOGGED
            case symbol_kind::S_UNTIL: // UNTIL
            case symbol_kind::S_UPDATE: // UPDATE
            case symbol_kind::S_USER: // USER
            case symbol_kind::S_USING: // USING
            case symbol_kind::S_VACUUM: // VACUUM
            case symbol_kind::S_VALID: // VALID
            case symbol_kind::S_VALIDATE: // VALIDATE
            case symbol_kind::S_VALIDATOR: // VALIDATOR
            case symbol_kind::S_VALUE_P: // VALUE_P
            case symbol_kind::S_VALUES: // VALUES
            case symbol_kind::S_VARCHAR: // VARCHAR
            case symbol_kind::S_VARIADIC: // VARIADIC
            case symbol_kind::S_VARYING: // VARYING
            case symbol_kind::S_VERBOSE: // VERBOSE
            case symbol_kind::S_VERSION_P: // VERSION_P
            case symbol_kind::S_VIEW: // VIEW
            case symbol_kind::S_VIEWS: // VIEWS
            case symbol_kind::S_VIRTUAL: // VIRTUAL
            case symbol_kind::S_VOLATILE: // VOLATILE
            case symbol_kind::S_WHEN: // WHEN
            case symbol_kind::S_WHERE: // WHERE
            case symbol_kind::S_WHITESPACE_P: // WHITESPACE_P
            case symbol_kind::S_WINDOW: // WINDOW
            case symbol_kind::S_WITH: // WITH
            case symbol_kind::S_WITHIN: // WITHIN
            case symbol_kind::S_WITHOUT: // WITHOUT
            case symbol_kind::S_WORK: // WORK
            case symbol_kind::S_WRAPPER: // WRAPPER
            case symbol_kind::S_WRITE: // WRITE
            case symbol_kind::S_XML_P: // XML_P
            case symbol_kind::S_XMLATTRIBUTES: // XMLATTRIBUTES
            case symbol_kind::S_XMLCONCAT: // XMLCONCAT
            case symbol_kind::S_XMLELEMENT: // XMLELEMENT
            case symbol_kind::S_XMLEXISTS: // XMLEXISTS
            case symbol_kind::S_XMLFOREST: // XMLFOREST
            case symbol_kind::S_XMLNAMESPACES: // XMLNAMESPACES
            case symbol_kind::S_XMLPARSE: // XMLPARSE
            case symbol_kind::S_XMLPI: // XMLPI
            case symbol_kind::S_XMLROOT: // XMLROOT
            case symbol_kind::S_XMLSERIALIZE: // XMLSERIALIZE
            case symbol_kind::S_XMLTABLE: // XMLTABLE
            case symbol_kind::S_YEAR_P: // YEAR_P
            case symbol_kind::S_YES_P: // YES_P
            case symbol_kind::S_ZONE: // ZONE
            case symbol_kind::S_copy_file_name: // copy_file_name
            case symbol_kind::S_copy_delimiter: // copy_delimiter
            case symbol_kind::S_var_name: // var_name
            case symbol_kind::S_NonReservedWord: // NonReservedWord
            case symbol_kind::S_relation_expr: // relation_expr
            case symbol_kind::S_any_operator: // any_operator
            case symbol_kind::S_qual_Op: // qual_Op
            case symbol_kind::S_all_Op: // all_Op
            case symbol_kind::S_ColId: // ColId
            case symbol_kind::S_type_function_name: // type_function_name
            case symbol_kind::S_type_func_name_keyword: // type_func_name_keyword
            case symbol_kind::S_ColLabel: // ColLabel
            case symbol_kind::S_BareColLabel: // BareColLabel
            case symbol_kind::S_reserved_keyword: // reserved_keyword
            case symbol_kind::S_unreserved_keyword: // unreserved_keyword
            case symbol_kind::S_col_name_keyword: // col_name_keyword
            case symbol_kind::S_qualified_name: // qualified_name
            case symbol_kind::S_name: // name
            case symbol_kind::S_attr_name: // attr_name
            case symbol_kind::S_func_name: // func_name
            case symbol_kind::S_insert_target: // insert_target
            case symbol_kind::S_insert_column_item: // insert_column_item
               value.template destroy<std::string>();
               break;

            case symbol_kind::S_in_expr: // in_expr
               value.template destroy<std::variant<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>, std::shared_ptr<lingodb::ast::SubqueryExpression>>>();
               break;

            case symbol_kind::S_when_clause_list: // when_clause_list
               value.template destroy<std::vector<lingodb::ast::CaseExpression::CaseCheck>>();
               break;

            case symbol_kind::S_copy_options: // copy_options
            case symbol_kind::S_copy_opt_list: // copy_opt_list
               value.template destroy<std::vector<std::pair<std::string, std::string>>>();
               break;

            case symbol_kind::S_stmtmulti: // stmtmulti
               value.template destroy<std::vector<std::shared_ptr<lingodb::ast::AstNode>>>();
               break;

            case symbol_kind::S_ColQualList: // ColQualList
               value.template destroy<std::vector<std::shared_ptr<lingodb::ast::Constraint>>>();
               break;

            case symbol_kind::S_func_expr_list: // func_expr_list
               value.template destroy<std::vector<std::shared_ptr<lingodb::ast::FunctionExpression>>>();
               break;

            case symbol_kind::S_sortby_list: // sortby_list
               value.template destroy<std::vector<std::shared_ptr<lingodb::ast::OrderByElement>>>();
               break;

            case symbol_kind::S_var_list: // var_list
            case symbol_kind::S_group_by_list: // group_by_list
            case symbol_kind::S_group_by_list_with_alias: // group_by_list_with_alias
            case symbol_kind::S_rollup_clause: // rollup_clause
            case symbol_kind::S_columnref_list: // columnref_list
            case symbol_kind::S_func_arg_list_opt: // func_arg_list_opt
            case symbol_kind::S_func_arg_list: // func_arg_list
            case symbol_kind::S_expr_list: // expr_list
            case symbol_kind::S_expr_list_with_alias: // expr_list_with_alias
            case symbol_kind::S_extract_list: // extract_list
            case symbol_kind::S_substr_list: // substr_list
            case symbol_kind::S_target_list: // target_list
            case symbol_kind::S_opt_partition_clause: // opt_partition_clause
               value.template destroy<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>>();
               break;

            case symbol_kind::S_OptTableElementList: // OptTableElementList
            case symbol_kind::S_TableElementList: // TableElementList
               value.template destroy<std::vector<std::shared_ptr<lingodb::ast::TableElement>>>();
               break;

            case symbol_kind::S_opt_type_modifiers: // opt_type_modifiers
            case symbol_kind::S_type_modifiers: // type_modifiers
               value.template destroy<std::vector<std::shared_ptr<lingodb::ast::Value>>>();
               break;

            case symbol_kind::S_opt_name_list: // opt_name_list
            case symbol_kind::S_name_list: // name_list
            case symbol_kind::S_insert_column_list: // insert_column_list
               value.template destroy<std::vector<std::string>>();
               break;

            case symbol_kind::S_INTEGER_VALUE: // "integer_value"
               value.template destroy<uint64_t>();
               break;

            default:
               break;
         }

         Base::clear();
      }

      /// The user-facing name of this symbol.
      const char* name() const YY_NOEXCEPT {
         return parser::symbol_name(this->kind());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get() const YY_NOEXCEPT;

      /// Whether empty.
      bool empty() const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move(basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

      private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator=(const basic_symbol& that);
#endif
   };

   /// Type access provider for token (enum) based symbols.
   struct by_kind {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind() YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind(by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind(const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind(kind_type t) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear() YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move(by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind() const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get() const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
   };

   /// Backward compatibility for a private implementation detail (Bison 3.6).
   typedef by_kind by_type;

   /// "External" symbols: returned by the scanner.
   struct symbol_type : basic_symbol<by_kind> {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type() YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type(int tok, location_type l)
         : super_type(token_kind_type(tok), std::move(l))
#else
      symbol_type(int tok, const location_type& l)
         : super_type(token_kind_type(tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
         YY_ASSERT(tok == token::TOK_YYEOF || (token::TOK_YYerror <= tok && tok <= token::TOK_YYUNDEF) || (token::TOK_LP <= tok && tok <= token::TOK_DOT) || (token::TOK_COMMA <= tok && tok <= token::TOK_SEMICOLON) || (token::TOK_HAT <= tok && tok <= token::TOK_PIPE) || tok == token::TOK_PARAM || (token::TOK_DOT_DOT <= tok && tok <= token::TOK_COLON_EQUALS) || (token::TOK_FORMAT_LA <= tok && tok <= token::TOK_ROLLUP_PRIORITY));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type(int tok, int v, location_type l)
         : super_type(token_kind_type(tok), std::move(v), std::move(l))
#else
      symbol_type(int tok, const int& v, const location_type& l)
         : super_type(token_kind_type(tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
         YY_ASSERT(tok == token::TOK_ICONST);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type(int tok, std::string v, location_type l)
         : super_type(token_kind_type(tok), std::move(v), std::move(l))
#else
      symbol_type(int tok, const std::string& v, const location_type& l)
         : super_type(token_kind_type(tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
         YY_ASSERT((token::TOK_FCONST <= tok && tok <= token::TOK_NATIONAL_VALUE) || tok == token::TOK_PERCENT || (token::TOK_PLUS <= tok && tok <= token::TOK_GREATER) || (token::TOK_UIDENT <= tok && tok <= token::TOK_Op) || tok == token::TOK_TYPECAST || (token::TOK_ABORT_P <= tok && tok <= token::TOK_ZONE));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type(int tok, uint64_t v, location_type l)
         : super_type(token_kind_type(tok), std::move(v), std::move(l))
#else
      symbol_type(int tok, const uint64_t& v, const location_type& l)
         : super_type(token_kind_type(tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
         YY_ASSERT(tok == token::TOK_INTEGER_VALUE);
#endif
      }
   };

   /// Build a parser object.
   parser(Driver& drv_yyarg);
   virtual ~parser();

#if 201103L <= YY_CPLUSPLUS
   /// Non copyable.
   parser(const parser&) = delete;
   /// Non copyable.
   parser& operator=(const parser&) = delete;
#endif

   /// Parse.  An alias for parse ().
   /// \returns  0 iff parsing succeeded.
   int operator()();

   /// Parse.
   /// \returns  0 iff parsing succeeded.
   virtual int parse();

#if YYDEBUG
   /// The current debugging stream.
   std::ostream& debug_stream() const YY_ATTRIBUTE_PURE;
   /// Set the current debugging stream.
   void set_debug_stream(std::ostream&);

   /// Type for debugging levels.
   typedef int debug_level_type;
   /// The current debugging level.
   debug_level_type debug_level() const YY_ATTRIBUTE_PURE;
   /// Set the current debugging level.
   void set_debug_level(debug_level_type l);
#endif

   /// Report a syntax error.
   /// \param loc    where the syntax error is found.
   /// \param msg    a description of the syntax error.
   virtual void error(const location_type& loc, const std::string& msg);

   /// Report a syntax error.
   void error(const syntax_error& err);

   /// The user-facing name of the symbol whose (internal) number is
   /// YYSYMBOL.  No bounds checking.
   static const char* symbol_name(symbol_kind_type yysymbol);

   // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_YYEOF(location_type l) {
      return symbol_type(token::TOK_YYEOF, std::move(l));
   }
#else
   static symbol_type
   make_YYEOF(const location_type& l) {
      return symbol_type(token::TOK_YYEOF, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_YYerror(location_type l) {
      return symbol_type(token::TOK_YYerror, std::move(l));
   }
#else
   static symbol_type
   make_YYerror(const location_type& l) {
      return symbol_type(token::TOK_YYerror, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_YYUNDEF(location_type l) {
      return symbol_type(token::TOK_YYUNDEF, std::move(l));
   }
#else
   static symbol_type
   make_YYUNDEF(const location_type& l) {
      return symbol_type(token::TOK_YYUNDEF, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ICONST(int v, location_type l) {
      return symbol_type(token::TOK_ICONST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ICONST(const int& v, const location_type& l) {
      return symbol_type(token::TOK_ICONST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INTEGER_VALUE(uint64_t v, location_type l) {
      return symbol_type(token::TOK_INTEGER_VALUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INTEGER_VALUE(const uint64_t& v, const location_type& l) {
      return symbol_type(token::TOK_INTEGER_VALUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FCONST(std::string v, location_type l) {
      return symbol_type(token::TOK_FCONST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FCONST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FCONST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IDENT(std::string v, location_type l) {
      return symbol_type(token::TOK_IDENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IDENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IDENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IDENTIFIER(std::string v, location_type l) {
      return symbol_type(token::TOK_IDENTIFIER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IDENTIFIER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IDENTIFIER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STRING_VALUE(std::string v, location_type l) {
      return symbol_type(token::TOK_STRING_VALUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STRING_VALUE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STRING_VALUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BIT_VALUE(std::string v, location_type l) {
      return symbol_type(token::TOK_BIT_VALUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BIT_VALUE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BIT_VALUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_HEX_VALUE(std::string v, location_type l) {
      return symbol_type(token::TOK_HEX_VALUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_HEX_VALUE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_HEX_VALUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NATIONAL_VALUE(std::string v, location_type l) {
      return symbol_type(token::TOK_NATIONAL_VALUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NATIONAL_VALUE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NATIONAL_VALUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LP(location_type l) {
      return symbol_type(token::TOK_LP, std::move(l));
   }
#else
   static symbol_type
   make_LP(const location_type& l) {
      return symbol_type(token::TOK_LP, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RP(location_type l) {
      return symbol_type(token::TOK_RP, std::move(l));
   }
#else
   static symbol_type
   make_RP(const location_type& l) {
      return symbol_type(token::TOK_RP, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LB(location_type l) {
      return symbol_type(token::TOK_LB, std::move(l));
   }
#else
   static symbol_type
   make_LB(const location_type& l) {
      return symbol_type(token::TOK_LB, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RB(location_type l) {
      return symbol_type(token::TOK_RB, std::move(l));
   }
#else
   static symbol_type
   make_RB(const location_type& l) {
      return symbol_type(token::TOK_RB, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DOT(location_type l) {
      return symbol_type(token::TOK_DOT, std::move(l));
   }
#else
   static symbol_type
   make_DOT(const location_type& l) {
      return symbol_type(token::TOK_DOT, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PERCENT(std::string v, location_type l) {
      return symbol_type(token::TOK_PERCENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PERCENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PERCENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COMMA(location_type l) {
      return symbol_type(token::TOK_COMMA, std::move(l));
   }
#else
   static symbol_type
   make_COMMA(const location_type& l) {
      return symbol_type(token::TOK_COMMA, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SEMICOLON(location_type l) {
      return symbol_type(token::TOK_SEMICOLON, std::move(l));
   }
#else
   static symbol_type
   make_SEMICOLON(const location_type& l) {
      return symbol_type(token::TOK_SEMICOLON, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PLUS(std::string v, location_type l) {
      return symbol_type(token::TOK_PLUS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PLUS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PLUS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SLASH(std::string v, location_type l) {
      return symbol_type(token::TOK_SLASH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SLASH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SLASH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STAR(std::string v, location_type l) {
      return symbol_type(token::TOK_STAR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STAR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STAR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MINUS(std::string v, location_type l) {
      return symbol_type(token::TOK_MINUS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MINUS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MINUS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EQUAL(std::string v, location_type l) {
      return symbol_type(token::TOK_EQUAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EQUAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EQUAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NOT_EQUAL(std::string v, location_type l) {
      return symbol_type(token::TOK_NOT_EQUAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NOT_EQUAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NOT_EQUAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LESS_EQUAL(std::string v, location_type l) {
      return symbol_type(token::TOK_LESS_EQUAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LESS_EQUAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LESS_EQUAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LESS(std::string v, location_type l) {
      return symbol_type(token::TOK_LESS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LESS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LESS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GREATER_EQUAL(std::string v, location_type l) {
      return symbol_type(token::TOK_GREATER_EQUAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GREATER_EQUAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GREATER_EQUAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GREATER(std::string v, location_type l) {
      return symbol_type(token::TOK_GREATER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GREATER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GREATER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_HAT(location_type l) {
      return symbol_type(token::TOK_HAT, std::move(l));
   }
#else
   static symbol_type
   make_HAT(const location_type& l) {
      return symbol_type(token::TOK_HAT, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_QUOTE(location_type l) {
      return symbol_type(token::TOK_QUOTE, std::move(l));
   }
#else
   static symbol_type
   make_QUOTE(const location_type& l) {
      return symbol_type(token::TOK_QUOTE, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PIPE(location_type l) {
      return symbol_type(token::TOK_PIPE, std::move(l));
   }
#else
   static symbol_type
   make_PIPE(const location_type& l) {
      return symbol_type(token::TOK_PIPE, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UIDENT(std::string v, location_type l) {
      return symbol_type(token::TOK_UIDENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UIDENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UIDENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SCONST(std::string v, location_type l) {
      return symbol_type(token::TOK_SCONST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SCONST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SCONST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_USCONST(std::string v, location_type l) {
      return symbol_type(token::TOK_USCONST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_USCONST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_USCONST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BCONST(std::string v, location_type l) {
      return symbol_type(token::TOK_BCONST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BCONST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BCONST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XCONST(std::string v, location_type l) {
      return symbol_type(token::TOK_XCONST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XCONST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XCONST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_Op(std::string v, location_type l) {
      return symbol_type(token::TOK_Op, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_Op(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_Op, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PARAM(location_type l) {
      return symbol_type(token::TOK_PARAM, std::move(l));
   }
#else
   static symbol_type
   make_PARAM(const location_type& l) {
      return symbol_type(token::TOK_PARAM, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TYPECAST(std::string v, location_type l) {
      return symbol_type(token::TOK_TYPECAST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TYPECAST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TYPECAST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DOT_DOT(location_type l) {
      return symbol_type(token::TOK_DOT_DOT, std::move(l));
   }
#else
   static symbol_type
   make_DOT_DOT(const location_type& l) {
      return symbol_type(token::TOK_DOT_DOT, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COLON_EQUALS(location_type l) {
      return symbol_type(token::TOK_COLON_EQUALS, std::move(l));
   }
#else
   static symbol_type
   make_COLON_EQUALS(const location_type& l) {
      return symbol_type(token::TOK_COLON_EQUALS, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ABORT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_ABORT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ABORT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ABORT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ABSENT(std::string v, location_type l) {
      return symbol_type(token::TOK_ABSENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ABSENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ABSENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ABSOLUTE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_ABSOLUTE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ABSOLUTE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ABSOLUTE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ACCESS(std::string v, location_type l) {
      return symbol_type(token::TOK_ACCESS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ACCESS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ACCESS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ACTION(std::string v, location_type l) {
      return symbol_type(token::TOK_ACTION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ACTION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ACTION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ADD_P(std::string v, location_type l) {
      return symbol_type(token::TOK_ADD_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ADD_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ADD_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ADMIN(std::string v, location_type l) {
      return symbol_type(token::TOK_ADMIN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ADMIN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ADMIN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_AFTER(std::string v, location_type l) {
      return symbol_type(token::TOK_AFTER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_AFTER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_AFTER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_AGGREGATE(std::string v, location_type l) {
      return symbol_type(token::TOK_AGGREGATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_AGGREGATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_AGGREGATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ALL(std::string v, location_type l) {
      return symbol_type(token::TOK_ALL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ALL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ALL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ALSO(std::string v, location_type l) {
      return symbol_type(token::TOK_ALSO, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ALSO(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ALSO, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ALTER(std::string v, location_type l) {
      return symbol_type(token::TOK_ALTER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ALTER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ALTER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ALWAYS(std::string v, location_type l) {
      return symbol_type(token::TOK_ALWAYS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ALWAYS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ALWAYS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ANALYSE(std::string v, location_type l) {
      return symbol_type(token::TOK_ANALYSE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ANALYSE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ANALYSE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ANALYZE(std::string v, location_type l) {
      return symbol_type(token::TOK_ANALYZE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ANALYZE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ANALYZE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_AND(std::string v, location_type l) {
      return symbol_type(token::TOK_AND, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_AND(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_AND, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ANY(std::string v, location_type l) {
      return symbol_type(token::TOK_ANY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ANY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ANY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ARRAY(std::string v, location_type l) {
      return symbol_type(token::TOK_ARRAY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ARRAY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ARRAY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_AS(std::string v, location_type l) {
      return symbol_type(token::TOK_AS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_AS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_AS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ASC(std::string v, location_type l) {
      return symbol_type(token::TOK_ASC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ASC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ASC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ASENSITIVE(std::string v, location_type l) {
      return symbol_type(token::TOK_ASENSITIVE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ASENSITIVE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ASENSITIVE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ASSERTION(std::string v, location_type l) {
      return symbol_type(token::TOK_ASSERTION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ASSERTION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ASSERTION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ASSIGNMENT(std::string v, location_type l) {
      return symbol_type(token::TOK_ASSIGNMENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ASSIGNMENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ASSIGNMENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ASYMMETRIC(std::string v, location_type l) {
      return symbol_type(token::TOK_ASYMMETRIC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ASYMMETRIC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ASYMMETRIC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ATOMIC(std::string v, location_type l) {
      return symbol_type(token::TOK_ATOMIC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ATOMIC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ATOMIC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_AT(std::string v, location_type l) {
      return symbol_type(token::TOK_AT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_AT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_AT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ATTACH(std::string v, location_type l) {
      return symbol_type(token::TOK_ATTACH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ATTACH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ATTACH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ATTRIBUTE(std::string v, location_type l) {
      return symbol_type(token::TOK_ATTRIBUTE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ATTRIBUTE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ATTRIBUTE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_AUTHORIZATION(std::string v, location_type l) {
      return symbol_type(token::TOK_AUTHORIZATION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_AUTHORIZATION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_AUTHORIZATION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BACKWARD(std::string v, location_type l) {
      return symbol_type(token::TOK_BACKWARD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BACKWARD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BACKWARD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BEFORE(std::string v, location_type l) {
      return symbol_type(token::TOK_BEFORE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BEFORE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BEFORE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BEGIN_P(std::string v, location_type l) {
      return symbol_type(token::TOK_BEGIN_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BEGIN_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BEGIN_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BETWEEN(std::string v, location_type l) {
      return symbol_type(token::TOK_BETWEEN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BETWEEN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BETWEEN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BIGINT(std::string v, location_type l) {
      return symbol_type(token::TOK_BIGINT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BIGINT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BIGINT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BINARY(std::string v, location_type l) {
      return symbol_type(token::TOK_BINARY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BINARY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BINARY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BIT(std::string v, location_type l) {
      return symbol_type(token::TOK_BIT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BIT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BIT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BOOLEAN_P(std::string v, location_type l) {
      return symbol_type(token::TOK_BOOLEAN_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BOOLEAN_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BOOLEAN_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BOTH(std::string v, location_type l) {
      return symbol_type(token::TOK_BOTH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BOTH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BOTH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BREADTH(std::string v, location_type l) {
      return symbol_type(token::TOK_BREADTH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BREADTH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BREADTH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_BY(std::string v, location_type l) {
      return symbol_type(token::TOK_BY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_BY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_BY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CACHE(std::string v, location_type l) {
      return symbol_type(token::TOK_CACHE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CACHE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CACHE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CALL(std::string v, location_type l) {
      return symbol_type(token::TOK_CALL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CALL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CALL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CALLED(std::string v, location_type l) {
      return symbol_type(token::TOK_CALLED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CALLED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CALLED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CASCADE(std::string v, location_type l) {
      return symbol_type(token::TOK_CASCADE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CASCADE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CASCADE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CASCADED(std::string v, location_type l) {
      return symbol_type(token::TOK_CASCADED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CASCADED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CASCADED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CASE(std::string v, location_type l) {
      return symbol_type(token::TOK_CASE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CASE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CASE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CAST(std::string v, location_type l) {
      return symbol_type(token::TOK_CAST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CAST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CAST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CATALOG_P(std::string v, location_type l) {
      return symbol_type(token::TOK_CATALOG_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CATALOG_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CATALOG_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CHAIN(std::string v, location_type l) {
      return symbol_type(token::TOK_CHAIN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CHAIN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CHAIN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CHAR_P(std::string v, location_type l) {
      return symbol_type(token::TOK_CHAR_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CHAR_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CHAR_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CHARACTER(std::string v, location_type l) {
      return symbol_type(token::TOK_CHARACTER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CHARACTER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CHARACTER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CHARACTERISTICS(std::string v, location_type l) {
      return symbol_type(token::TOK_CHARACTERISTICS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CHARACTERISTICS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CHARACTERISTICS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CHECK(std::string v, location_type l) {
      return symbol_type(token::TOK_CHECK, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CHECK(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CHECK, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CHECKPOINT(std::string v, location_type l) {
      return symbol_type(token::TOK_CHECKPOINT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CHECKPOINT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CHECKPOINT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CLASS(std::string v, location_type l) {
      return symbol_type(token::TOK_CLASS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CLASS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CLASS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CLOSE(std::string v, location_type l) {
      return symbol_type(token::TOK_CLOSE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CLOSE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CLOSE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CLUSTER(std::string v, location_type l) {
      return symbol_type(token::TOK_CLUSTER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CLUSTER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CLUSTER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COALESCE(std::string v, location_type l) {
      return symbol_type(token::TOK_COALESCE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COALESCE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COALESCE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COLLATE(std::string v, location_type l) {
      return symbol_type(token::TOK_COLLATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COLLATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COLLATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COLLATION(std::string v, location_type l) {
      return symbol_type(token::TOK_COLLATION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COLLATION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COLLATION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COLUMN(std::string v, location_type l) {
      return symbol_type(token::TOK_COLUMN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COLUMN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COLUMN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COLUMNS(std::string v, location_type l) {
      return symbol_type(token::TOK_COLUMNS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COLUMNS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COLUMNS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COMMENT(std::string v, location_type l) {
      return symbol_type(token::TOK_COMMENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COMMENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COMMENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COMMENTS(std::string v, location_type l) {
      return symbol_type(token::TOK_COMMENTS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COMMENTS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COMMENTS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COMMIT(std::string v, location_type l) {
      return symbol_type(token::TOK_COMMIT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COMMIT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COMMIT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COMMITTED(std::string v, location_type l) {
      return symbol_type(token::TOK_COMMITTED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COMMITTED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COMMITTED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COMPRESSION(std::string v, location_type l) {
      return symbol_type(token::TOK_COMPRESSION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COMPRESSION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COMPRESSION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONCURRENTLY(std::string v, location_type l) {
      return symbol_type(token::TOK_CONCURRENTLY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONCURRENTLY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONCURRENTLY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONDITIONAL(std::string v, location_type l) {
      return symbol_type(token::TOK_CONDITIONAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONDITIONAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONDITIONAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONFIGURATION(std::string v, location_type l) {
      return symbol_type(token::TOK_CONFIGURATION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONFIGURATION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONFIGURATION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONFLICT(std::string v, location_type l) {
      return symbol_type(token::TOK_CONFLICT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONFLICT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONFLICT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONNECTION(std::string v, location_type l) {
      return symbol_type(token::TOK_CONNECTION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONNECTION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONNECTION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONSTRAINT(std::string v, location_type l) {
      return symbol_type(token::TOK_CONSTRAINT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONSTRAINT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONSTRAINT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONSTRAINTS(std::string v, location_type l) {
      return symbol_type(token::TOK_CONSTRAINTS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONSTRAINTS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONSTRAINTS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONTENT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_CONTENT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONTENT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONTENT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONTINUE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_CONTINUE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONTINUE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONTINUE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CONVERSION_P(std::string v, location_type l) {
      return symbol_type(token::TOK_CONVERSION_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CONVERSION_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CONVERSION_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COPY(std::string v, location_type l) {
      return symbol_type(token::TOK_COPY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COPY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COPY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_COST(std::string v, location_type l) {
      return symbol_type(token::TOK_COST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_COST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_COST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CREATE(std::string v, location_type l) {
      return symbol_type(token::TOK_CREATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CREATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CREATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CROSS(std::string v, location_type l) {
      return symbol_type(token::TOK_CROSS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CROSS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CROSS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CSV(std::string v, location_type l) {
      return symbol_type(token::TOK_CSV, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CSV(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CSV, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CUBE(std::string v, location_type l) {
      return symbol_type(token::TOK_CUBE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CUBE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CUBE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURRENT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_CURRENT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURRENT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURRENT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURRENT_CATALOG(std::string v, location_type l) {
      return symbol_type(token::TOK_CURRENT_CATALOG, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURRENT_CATALOG(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURRENT_CATALOG, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURRENT_DATE(std::string v, location_type l) {
      return symbol_type(token::TOK_CURRENT_DATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURRENT_DATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURRENT_DATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURRENT_ROLE(std::string v, location_type l) {
      return symbol_type(token::TOK_CURRENT_ROLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURRENT_ROLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURRENT_ROLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURRENT_SCHEMA(std::string v, location_type l) {
      return symbol_type(token::TOK_CURRENT_SCHEMA, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURRENT_SCHEMA(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURRENT_SCHEMA, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURRENT_TIME(std::string v, location_type l) {
      return symbol_type(token::TOK_CURRENT_TIME, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURRENT_TIME(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURRENT_TIME, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURRENT_TIMESTAMP(std::string v, location_type l) {
      return symbol_type(token::TOK_CURRENT_TIMESTAMP, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURRENT_TIMESTAMP(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURRENT_TIMESTAMP, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURRENT_USER(std::string v, location_type l) {
      return symbol_type(token::TOK_CURRENT_USER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURRENT_USER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURRENT_USER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CURSOR(std::string v, location_type l) {
      return symbol_type(token::TOK_CURSOR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CURSOR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CURSOR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_CYCLE(std::string v, location_type l) {
      return symbol_type(token::TOK_CYCLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_CYCLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_CYCLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DATA_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DATA_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DATA_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DATA_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DATABASE(std::string v, location_type l) {
      return symbol_type(token::TOK_DATABASE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DATABASE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DATABASE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DAY_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DAY_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DAY_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DAY_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEALLOCATE(std::string v, location_type l) {
      return symbol_type(token::TOK_DEALLOCATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEALLOCATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEALLOCATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEC(std::string v, location_type l) {
      return symbol_type(token::TOK_DEC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DECIMAL_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DECIMAL_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DECIMAL_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DECIMAL_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DECLARE(std::string v, location_type l) {
      return symbol_type(token::TOK_DECLARE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DECLARE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DECLARE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEFAULT(std::string v, location_type l) {
      return symbol_type(token::TOK_DEFAULT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEFAULT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEFAULT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEFAULTS(std::string v, location_type l) {
      return symbol_type(token::TOK_DEFAULTS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEFAULTS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEFAULTS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DATE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DATE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DATE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DATE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEFERRABLE(std::string v, location_type l) {
      return symbol_type(token::TOK_DEFERRABLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEFERRABLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEFERRABLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEFERRED(std::string v, location_type l) {
      return symbol_type(token::TOK_DEFERRED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEFERRED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEFERRED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEFINER(std::string v, location_type l) {
      return symbol_type(token::TOK_DEFINER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEFINER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEFINER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DELETE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DELETE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DELETE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DELETE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DELIMITER(std::string v, location_type l) {
      return symbol_type(token::TOK_DELIMITER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DELIMITER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DELIMITER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DELIMITERS(std::string v, location_type l) {
      return symbol_type(token::TOK_DELIMITERS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DELIMITERS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DELIMITERS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEPENDS(std::string v, location_type l) {
      return symbol_type(token::TOK_DEPENDS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEPENDS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEPENDS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DEPTH(std::string v, location_type l) {
      return symbol_type(token::TOK_DEPTH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DEPTH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DEPTH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DESC(std::string v, location_type l) {
      return symbol_type(token::TOK_DESC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DESC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DESC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DETACH(std::string v, location_type l) {
      return symbol_type(token::TOK_DETACH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DETACH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DETACH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DICTIONARY(std::string v, location_type l) {
      return symbol_type(token::TOK_DICTIONARY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DICTIONARY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DICTIONARY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DISABLE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DISABLE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DISABLE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DISABLE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DISCARD(std::string v, location_type l) {
      return symbol_type(token::TOK_DISCARD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DISCARD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DISCARD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DISTINCT(std::string v, location_type l) {
      return symbol_type(token::TOK_DISTINCT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DISTINCT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DISTINCT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DO(std::string v, location_type l) {
      return symbol_type(token::TOK_DO, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DO(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DO, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DOCUMENT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DOCUMENT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DOCUMENT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DOCUMENT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DOMAIN_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DOMAIN_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DOMAIN_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DOMAIN_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DOUBLE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_DOUBLE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DOUBLE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DOUBLE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_DROP(std::string v, location_type l) {
      return symbol_type(token::TOK_DROP, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_DROP(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_DROP, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EACH(std::string v, location_type l) {
      return symbol_type(token::TOK_EACH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EACH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EACH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ELSE(std::string v, location_type l) {
      return symbol_type(token::TOK_ELSE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ELSE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ELSE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EMPTY_P(std::string v, location_type l) {
      return symbol_type(token::TOK_EMPTY_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EMPTY_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EMPTY_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ENABLE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_ENABLE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ENABLE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ENABLE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ENCODING(std::string v, location_type l) {
      return symbol_type(token::TOK_ENCODING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ENCODING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ENCODING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ENCRYPTED(std::string v, location_type l) {
      return symbol_type(token::TOK_ENCRYPTED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ENCRYPTED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ENCRYPTED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_END_P(std::string v, location_type l) {
      return symbol_type(token::TOK_END_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_END_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_END_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ENFORCED(std::string v, location_type l) {
      return symbol_type(token::TOK_ENFORCED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ENFORCED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ENFORCED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ENUM_P(std::string v, location_type l) {
      return symbol_type(token::TOK_ENUM_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ENUM_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ENUM_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ERROR_P(std::string v, location_type l) {
      return symbol_type(token::TOK_ERROR_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ERROR_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ERROR_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ESCAPE(std::string v, location_type l) {
      return symbol_type(token::TOK_ESCAPE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ESCAPE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ESCAPE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EVENT(std::string v, location_type l) {
      return symbol_type(token::TOK_EVENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EVENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EVENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXCEPT(std::string v, location_type l) {
      return symbol_type(token::TOK_EXCEPT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXCEPT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXCEPT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXCLUDE(std::string v, location_type l) {
      return symbol_type(token::TOK_EXCLUDE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXCLUDE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXCLUDE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXCLUDING(std::string v, location_type l) {
      return symbol_type(token::TOK_EXCLUDING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXCLUDING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXCLUDING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXCLUSIVE(std::string v, location_type l) {
      return symbol_type(token::TOK_EXCLUSIVE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXCLUSIVE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXCLUSIVE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXECUTE(std::string v, location_type l) {
      return symbol_type(token::TOK_EXECUTE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXECUTE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXECUTE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXISTS(std::string v, location_type l) {
      return symbol_type(token::TOK_EXISTS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXISTS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXISTS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXPLAIN(std::string v, location_type l) {
      return symbol_type(token::TOK_EXPLAIN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXPLAIN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXPLAIN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXPRESSION(std::string v, location_type l) {
      return symbol_type(token::TOK_EXPRESSION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXPRESSION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXPRESSION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXTENSION(std::string v, location_type l) {
      return symbol_type(token::TOK_EXTENSION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXTENSION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXTENSION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXTERNAL(std::string v, location_type l) {
      return symbol_type(token::TOK_EXTERNAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXTERNAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXTERNAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXTRACT(std::string v, location_type l) {
      return symbol_type(token::TOK_EXTRACT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXTRACT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXTRACT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FALSE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_FALSE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FALSE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FALSE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FAMILY(std::string v, location_type l) {
      return symbol_type(token::TOK_FAMILY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FAMILY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FAMILY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FETCH(std::string v, location_type l) {
      return symbol_type(token::TOK_FETCH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FETCH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FETCH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FILTER(std::string v, location_type l) {
      return symbol_type(token::TOK_FILTER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FILTER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FILTER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FINALIZE(std::string v, location_type l) {
      return symbol_type(token::TOK_FINALIZE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FINALIZE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FINALIZE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FIRST_P(std::string v, location_type l) {
      return symbol_type(token::TOK_FIRST_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FIRST_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FIRST_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FLOAT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_FLOAT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FLOAT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FLOAT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FOLLOWING(std::string v, location_type l) {
      return symbol_type(token::TOK_FOLLOWING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FOLLOWING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FOLLOWING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FOR(std::string v, location_type l) {
      return symbol_type(token::TOK_FOR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FOR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FOR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FORCE(std::string v, location_type l) {
      return symbol_type(token::TOK_FORCE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FORCE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FORCE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FOREIGN(std::string v, location_type l) {
      return symbol_type(token::TOK_FOREIGN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FOREIGN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FOREIGN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FORMAT(std::string v, location_type l) {
      return symbol_type(token::TOK_FORMAT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FORMAT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FORMAT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FORWARD(std::string v, location_type l) {
      return symbol_type(token::TOK_FORWARD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FORWARD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FORWARD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FREEZE(std::string v, location_type l) {
      return symbol_type(token::TOK_FREEZE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FREEZE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FREEZE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FROM(std::string v, location_type l) {
      return symbol_type(token::TOK_FROM, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FROM(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FROM, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FULL(std::string v, location_type l) {
      return symbol_type(token::TOK_FULL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FULL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FULL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FUNCTION(std::string v, location_type l) {
      return symbol_type(token::TOK_FUNCTION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FUNCTION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FUNCTION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FUNCTIONS(std::string v, location_type l) {
      return symbol_type(token::TOK_FUNCTIONS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_FUNCTIONS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_FUNCTIONS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GENERATED(std::string v, location_type l) {
      return symbol_type(token::TOK_GENERATED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GENERATED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GENERATED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GLOBAL(std::string v, location_type l) {
      return symbol_type(token::TOK_GLOBAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GLOBAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GLOBAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GRANT(std::string v, location_type l) {
      return symbol_type(token::TOK_GRANT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GRANT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GRANT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GRANTED(std::string v, location_type l) {
      return symbol_type(token::TOK_GRANTED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GRANTED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GRANTED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GREATEST(std::string v, location_type l) {
      return symbol_type(token::TOK_GREATEST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GREATEST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GREATEST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GROUP_P(std::string v, location_type l) {
      return symbol_type(token::TOK_GROUP_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GROUP_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GROUP_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GROUPING(std::string v, location_type l) {
      return symbol_type(token::TOK_GROUPING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GROUPING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GROUPING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_GROUPS(std::string v, location_type l) {
      return symbol_type(token::TOK_GROUPS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_GROUPS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_GROUPS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_HANDLER(std::string v, location_type l) {
      return symbol_type(token::TOK_HANDLER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_HANDLER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_HANDLER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_HAVING(std::string v, location_type l) {
      return symbol_type(token::TOK_HAVING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_HAVING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_HAVING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_HEADER_P(std::string v, location_type l) {
      return symbol_type(token::TOK_HEADER_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_HEADER_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_HEADER_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_HOLD(std::string v, location_type l) {
      return symbol_type(token::TOK_HOLD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_HOLD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_HOLD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_HOUR_P(std::string v, location_type l) {
      return symbol_type(token::TOK_HOUR_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_HOUR_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_HOUR_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IDENTITY_P(std::string v, location_type l) {
      return symbol_type(token::TOK_IDENTITY_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IDENTITY_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IDENTITY_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IF_P(std::string v, location_type l) {
      return symbol_type(token::TOK_IF_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IF_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IF_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ILIKE(std::string v, location_type l) {
      return symbol_type(token::TOK_ILIKE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ILIKE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ILIKE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IMMEDIATE(std::string v, location_type l) {
      return symbol_type(token::TOK_IMMEDIATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IMMEDIATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IMMEDIATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IMMUTABLE(std::string v, location_type l) {
      return symbol_type(token::TOK_IMMUTABLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IMMUTABLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IMMUTABLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IMPLICIT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_IMPLICIT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IMPLICIT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IMPLICIT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IMPORT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_IMPORT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IMPORT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IMPORT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IN_P(std::string v, location_type l) {
      return symbol_type(token::TOK_IN_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IN_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IN_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INCLUDE(std::string v, location_type l) {
      return symbol_type(token::TOK_INCLUDE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INCLUDE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INCLUDE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INCLUDING(std::string v, location_type l) {
      return symbol_type(token::TOK_INCLUDING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INCLUDING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INCLUDING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INCREMENT(std::string v, location_type l) {
      return symbol_type(token::TOK_INCREMENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INCREMENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INCREMENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INDENT(std::string v, location_type l) {
      return symbol_type(token::TOK_INDENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INDENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INDENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INDEX(std::string v, location_type l) {
      return symbol_type(token::TOK_INDEX, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INDEX(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INDEX, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INDEXES(std::string v, location_type l) {
      return symbol_type(token::TOK_INDEXES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INDEXES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INDEXES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INHERIT(std::string v, location_type l) {
      return symbol_type(token::TOK_INHERIT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INHERIT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INHERIT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INHERITS(std::string v, location_type l) {
      return symbol_type(token::TOK_INHERITS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INHERITS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INHERITS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INITIALLY(std::string v, location_type l) {
      return symbol_type(token::TOK_INITIALLY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INITIALLY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INITIALLY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INLINE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_INLINE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INLINE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INLINE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INNER_P(std::string v, location_type l) {
      return symbol_type(token::TOK_INNER_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INNER_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INNER_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INOUT(std::string v, location_type l) {
      return symbol_type(token::TOK_INOUT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INOUT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INOUT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INPUT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_INPUT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INPUT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INPUT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INSENSITIVE(std::string v, location_type l) {
      return symbol_type(token::TOK_INSENSITIVE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INSENSITIVE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INSENSITIVE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INSERT(std::string v, location_type l) {
      return symbol_type(token::TOK_INSERT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INSERT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INSERT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INSTEAD(std::string v, location_type l) {
      return symbol_type(token::TOK_INSTEAD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INSTEAD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INSTEAD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_INT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INTEGER(std::string v, location_type l) {
      return symbol_type(token::TOK_INTEGER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INTEGER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INTEGER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INTERSECT(std::string v, location_type l) {
      return symbol_type(token::TOK_INTERSECT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INTERSECT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INTERSECT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INTERVAL(std::string v, location_type l) {
      return symbol_type(token::TOK_INTERVAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INTERVAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INTERVAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INTO(std::string v, location_type l) {
      return symbol_type(token::TOK_INTO, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INTO(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INTO, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_INVOKER(std::string v, location_type l) {
      return symbol_type(token::TOK_INVOKER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_INVOKER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_INVOKER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_IS(std::string v, location_type l) {
      return symbol_type(token::TOK_IS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_IS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_IS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ISNULL(std::string v, location_type l) {
      return symbol_type(token::TOK_ISNULL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ISNULL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ISNULL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ISOLATION(std::string v, location_type l) {
      return symbol_type(token::TOK_ISOLATION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ISOLATION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ISOLATION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JOIN(std::string v, location_type l) {
      return symbol_type(token::TOK_JOIN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JOIN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JOIN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_ARRAY(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_ARRAY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_ARRAY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_ARRAY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_ARRAYAGG(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_ARRAYAGG, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_ARRAYAGG(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_ARRAYAGG, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_EXISTS(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_EXISTS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_EXISTS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_EXISTS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_OBJECT(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_OBJECT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_OBJECT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_OBJECT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_OBJECTAGG(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_OBJECTAGG, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_OBJECTAGG(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_OBJECTAGG, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_QUERY(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_QUERY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_QUERY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_QUERY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_SCALAR(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_SCALAR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_SCALAR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_SCALAR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_SERIALIZE(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_SERIALIZE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_SERIALIZE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_SERIALIZE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_TABLE(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_TABLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_TABLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_TABLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_JSON_VALUE(std::string v, location_type l) {
      return symbol_type(token::TOK_JSON_VALUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_JSON_VALUE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_JSON_VALUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_KEEP(std::string v, location_type l) {
      return symbol_type(token::TOK_KEEP, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_KEEP(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_KEEP, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_KEY(std::string v, location_type l) {
      return symbol_type(token::TOK_KEY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_KEY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_KEY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_KEYS(std::string v, location_type l) {
      return symbol_type(token::TOK_KEYS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_KEYS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_KEYS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LABEL(std::string v, location_type l) {
      return symbol_type(token::TOK_LABEL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LABEL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LABEL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LANGUAGE(std::string v, location_type l) {
      return symbol_type(token::TOK_LANGUAGE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LANGUAGE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LANGUAGE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LARGE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_LARGE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LARGE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LARGE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LAST_P(std::string v, location_type l) {
      return symbol_type(token::TOK_LAST_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LAST_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LAST_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LATERAL_P(std::string v, location_type l) {
      return symbol_type(token::TOK_LATERAL_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LATERAL_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LATERAL_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LEADING(std::string v, location_type l) {
      return symbol_type(token::TOK_LEADING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LEADING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LEADING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LEAKPROOF(std::string v, location_type l) {
      return symbol_type(token::TOK_LEAKPROOF, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LEAKPROOF(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LEAKPROOF, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LEAST(std::string v, location_type l) {
      return symbol_type(token::TOK_LEAST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LEAST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LEAST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LEFT(std::string v, location_type l) {
      return symbol_type(token::TOK_LEFT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LEFT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LEFT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LEVEL(std::string v, location_type l) {
      return symbol_type(token::TOK_LEVEL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LEVEL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LEVEL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LIKE(std::string v, location_type l) {
      return symbol_type(token::TOK_LIKE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LIKE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LIKE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LIMIT(std::string v, location_type l) {
      return symbol_type(token::TOK_LIMIT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LIMIT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LIMIT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LISTEN(std::string v, location_type l) {
      return symbol_type(token::TOK_LISTEN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LISTEN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LISTEN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LOAD(std::string v, location_type l) {
      return symbol_type(token::TOK_LOAD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LOAD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LOAD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LOCAL(std::string v, location_type l) {
      return symbol_type(token::TOK_LOCAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LOCAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LOCAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LOCALTIME(std::string v, location_type l) {
      return symbol_type(token::TOK_LOCALTIME, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LOCALTIME(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LOCALTIME, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LOCALTIMESTAMP(std::string v, location_type l) {
      return symbol_type(token::TOK_LOCALTIMESTAMP, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LOCALTIMESTAMP(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LOCALTIMESTAMP, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LOCATION(std::string v, location_type l) {
      return symbol_type(token::TOK_LOCATION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LOCATION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LOCATION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LOCK_P(std::string v, location_type l) {
      return symbol_type(token::TOK_LOCK_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LOCK_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LOCK_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LOCKED(std::string v, location_type l) {
      return symbol_type(token::TOK_LOCKED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LOCKED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LOCKED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_LOGGED(std::string v, location_type l) {
      return symbol_type(token::TOK_LOGGED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_LOGGED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_LOGGED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MAPPING(std::string v, location_type l) {
      return symbol_type(token::TOK_MAPPING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MAPPING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MAPPING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MATCH(std::string v, location_type l) {
      return symbol_type(token::TOK_MATCH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MATCH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MATCH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MATCHED(std::string v, location_type l) {
      return symbol_type(token::TOK_MATCHED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MATCHED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MATCHED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MATERIALIZED(std::string v, location_type l) {
      return symbol_type(token::TOK_MATERIALIZED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MATERIALIZED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MATERIALIZED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MAXVALUE(std::string v, location_type l) {
      return symbol_type(token::TOK_MAXVALUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MAXVALUE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MAXVALUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MERGE(std::string v, location_type l) {
      return symbol_type(token::TOK_MERGE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MERGE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MERGE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MERGE_ACTION(std::string v, location_type l) {
      return symbol_type(token::TOK_MERGE_ACTION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MERGE_ACTION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MERGE_ACTION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_METHOD(std::string v, location_type l) {
      return symbol_type(token::TOK_METHOD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_METHOD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_METHOD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MINUTE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_MINUTE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MINUTE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MINUTE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MINVALUE(std::string v, location_type l) {
      return symbol_type(token::TOK_MINVALUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MINVALUE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MINVALUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MODE(std::string v, location_type l) {
      return symbol_type(token::TOK_MODE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MODE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MODE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MONTH_P(std::string v, location_type l) {
      return symbol_type(token::TOK_MONTH_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MONTH_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MONTH_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_MOVE(std::string v, location_type l) {
      return symbol_type(token::TOK_MOVE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_MOVE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_MOVE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NAME_P(std::string v, location_type l) {
      return symbol_type(token::TOK_NAME_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NAME_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NAME_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NAMES(std::string v, location_type l) {
      return symbol_type(token::TOK_NAMES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NAMES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NAMES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NATIONAL(std::string v, location_type l) {
      return symbol_type(token::TOK_NATIONAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NATIONAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NATIONAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NATURAL(std::string v, location_type l) {
      return symbol_type(token::TOK_NATURAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NATURAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NATURAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NCHAR(std::string v, location_type l) {
      return symbol_type(token::TOK_NCHAR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NCHAR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NCHAR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NESTED(std::string v, location_type l) {
      return symbol_type(token::TOK_NESTED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NESTED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NESTED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NEW(std::string v, location_type l) {
      return symbol_type(token::TOK_NEW, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NEW(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NEW, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NEXT(std::string v, location_type l) {
      return symbol_type(token::TOK_NEXT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NEXT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NEXT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NFC(std::string v, location_type l) {
      return symbol_type(token::TOK_NFC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NFC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NFC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NFD(std::string v, location_type l) {
      return symbol_type(token::TOK_NFD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NFD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NFD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NFKC(std::string v, location_type l) {
      return symbol_type(token::TOK_NFKC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NFKC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NFKC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NFKD(std::string v, location_type l) {
      return symbol_type(token::TOK_NFKD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NFKD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NFKD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NO(std::string v, location_type l) {
      return symbol_type(token::TOK_NO, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NO(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NO, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NONE(std::string v, location_type l) {
      return symbol_type(token::TOK_NONE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NONE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NONE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NORMALIZE(std::string v, location_type l) {
      return symbol_type(token::TOK_NORMALIZE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NORMALIZE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NORMALIZE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NORMALIZED(std::string v, location_type l) {
      return symbol_type(token::TOK_NORMALIZED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NORMALIZED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NORMALIZED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NOT(std::string v, location_type l) {
      return symbol_type(token::TOK_NOT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NOT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NOT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NOTHING(std::string v, location_type l) {
      return symbol_type(token::TOK_NOTHING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NOTHING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NOTHING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NOTIFY(std::string v, location_type l) {
      return symbol_type(token::TOK_NOTIFY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NOTIFY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NOTIFY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NOTNULL(std::string v, location_type l) {
      return symbol_type(token::TOK_NOTNULL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NOTNULL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NOTNULL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NOWAIT(std::string v, location_type l) {
      return symbol_type(token::TOK_NOWAIT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NOWAIT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NOWAIT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NULL_P(std::string v, location_type l) {
      return symbol_type(token::TOK_NULL_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NULL_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NULL_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NULLIF(std::string v, location_type l) {
      return symbol_type(token::TOK_NULLIF, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NULLIF(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NULLIF, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NULLS_P(std::string v, location_type l) {
      return symbol_type(token::TOK_NULLS_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NULLS_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NULLS_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NUMERIC(std::string v, location_type l) {
      return symbol_type(token::TOK_NUMERIC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_NUMERIC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_NUMERIC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OBJECT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_OBJECT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OBJECT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OBJECT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OBJECTS_P(std::string v, location_type l) {
      return symbol_type(token::TOK_OBJECTS_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OBJECTS_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OBJECTS_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OF(std::string v, location_type l) {
      return symbol_type(token::TOK_OF, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OF(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OF, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OFF(std::string v, location_type l) {
      return symbol_type(token::TOK_OFF, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OFF(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OFF, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OFFSET(std::string v, location_type l) {
      return symbol_type(token::TOK_OFFSET, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OFFSET(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OFFSET, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OIDS(std::string v, location_type l) {
      return symbol_type(token::TOK_OIDS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OIDS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OIDS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OLD(std::string v, location_type l) {
      return symbol_type(token::TOK_OLD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OLD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OLD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OMIT(std::string v, location_type l) {
      return symbol_type(token::TOK_OMIT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OMIT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OMIT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ON(std::string v, location_type l) {
      return symbol_type(token::TOK_ON, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ON(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ON, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ONLY(std::string v, location_type l) {
      return symbol_type(token::TOK_ONLY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ONLY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ONLY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OPERATOR(std::string v, location_type l) {
      return symbol_type(token::TOK_OPERATOR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OPERATOR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OPERATOR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OPTION(std::string v, location_type l) {
      return symbol_type(token::TOK_OPTION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OPTION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OPTION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OPTIONS(std::string v, location_type l) {
      return symbol_type(token::TOK_OPTIONS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OPTIONS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OPTIONS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OR(std::string v, location_type l) {
      return symbol_type(token::TOK_OR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ORDER(std::string v, location_type l) {
      return symbol_type(token::TOK_ORDER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ORDER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ORDER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ORDINALITY(std::string v, location_type l) {
      return symbol_type(token::TOK_ORDINALITY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ORDINALITY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ORDINALITY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OTHERS(std::string v, location_type l) {
      return symbol_type(token::TOK_OTHERS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OTHERS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OTHERS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OUT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_OUT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OUT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OUT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OUTER_P(std::string v, location_type l) {
      return symbol_type(token::TOK_OUTER_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OUTER_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OUTER_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OVER(std::string v, location_type l) {
      return symbol_type(token::TOK_OVER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OVER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OVER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OVERLAPS(std::string v, location_type l) {
      return symbol_type(token::TOK_OVERLAPS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OVERLAPS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OVERLAPS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OVERLAY(std::string v, location_type l) {
      return symbol_type(token::TOK_OVERLAY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OVERLAY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OVERLAY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OVERRIDING(std::string v, location_type l) {
      return symbol_type(token::TOK_OVERRIDING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OVERRIDING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OVERRIDING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OWNED(std::string v, location_type l) {
      return symbol_type(token::TOK_OWNED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OWNED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OWNED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OWNER(std::string v, location_type l) {
      return symbol_type(token::TOK_OWNER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_OWNER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_OWNER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PARALLEL(std::string v, location_type l) {
      return symbol_type(token::TOK_PARALLEL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PARALLEL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PARALLEL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PARAMETER(std::string v, location_type l) {
      return symbol_type(token::TOK_PARAMETER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PARAMETER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PARAMETER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PARSER(std::string v, location_type l) {
      return symbol_type(token::TOK_PARSER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PARSER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PARSER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PARTIAL(std::string v, location_type l) {
      return symbol_type(token::TOK_PARTIAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PARTIAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PARTIAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PARTITION(std::string v, location_type l) {
      return symbol_type(token::TOK_PARTITION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PARTITION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PARTITION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PASSING(std::string v, location_type l) {
      return symbol_type(token::TOK_PASSING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PASSING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PASSING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PASSWORD(std::string v, location_type l) {
      return symbol_type(token::TOK_PASSWORD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PASSWORD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PASSWORD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PATH(std::string v, location_type l) {
      return symbol_type(token::TOK_PATH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PATH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PATH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PERIOD(std::string v, location_type l) {
      return symbol_type(token::TOK_PERIOD, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PERIOD(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PERIOD, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PLACING(std::string v, location_type l) {
      return symbol_type(token::TOK_PLACING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PLACING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PLACING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PLAN(std::string v, location_type l) {
      return symbol_type(token::TOK_PLAN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PLAN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PLAN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PLANS(std::string v, location_type l) {
      return symbol_type(token::TOK_PLANS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PLANS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PLANS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_POLICY(std::string v, location_type l) {
      return symbol_type(token::TOK_POLICY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_POLICY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_POLICY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_POSITION(std::string v, location_type l) {
      return symbol_type(token::TOK_POSITION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_POSITION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_POSITION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PRECEDING(std::string v, location_type l) {
      return symbol_type(token::TOK_PRECEDING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PRECEDING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PRECEDING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PRECISION(std::string v, location_type l) {
      return symbol_type(token::TOK_PRECISION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PRECISION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PRECISION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PRESERVE(std::string v, location_type l) {
      return symbol_type(token::TOK_PRESERVE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PRESERVE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PRESERVE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PREPARE(std::string v, location_type l) {
      return symbol_type(token::TOK_PREPARE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PREPARE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PREPARE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PREPARED(std::string v, location_type l) {
      return symbol_type(token::TOK_PREPARED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PREPARED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PREPARED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PRIMARY(std::string v, location_type l) {
      return symbol_type(token::TOK_PRIMARY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PRIMARY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PRIMARY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PRIOR(std::string v, location_type l) {
      return symbol_type(token::TOK_PRIOR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PRIOR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PRIOR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PRIVILEGES(std::string v, location_type l) {
      return symbol_type(token::TOK_PRIVILEGES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PRIVILEGES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PRIVILEGES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PROCEDURAL(std::string v, location_type l) {
      return symbol_type(token::TOK_PROCEDURAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PROCEDURAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PROCEDURAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PROCEDURE(std::string v, location_type l) {
      return symbol_type(token::TOK_PROCEDURE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PROCEDURE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PROCEDURE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PROCEDURES(std::string v, location_type l) {
      return symbol_type(token::TOK_PROCEDURES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PROCEDURES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PROCEDURES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PROGRAM(std::string v, location_type l) {
      return symbol_type(token::TOK_PROGRAM, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PROGRAM(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PROGRAM, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_PUBLICATION(std::string v, location_type l) {
      return symbol_type(token::TOK_PUBLICATION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_PUBLICATION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_PUBLICATION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_QUOTES(std::string v, location_type l) {
      return symbol_type(token::TOK_QUOTES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_QUOTES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_QUOTES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_EXTEND(std::string v, location_type l) {
      return symbol_type(token::TOK_EXTEND, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_EXTEND(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_EXTEND, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RANGE(std::string v, location_type l) {
      return symbol_type(token::TOK_RANGE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RANGE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RANGE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_READ(std::string v, location_type l) {
      return symbol_type(token::TOK_READ, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_READ(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_READ, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REAL(std::string v, location_type l) {
      return symbol_type(token::TOK_REAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REASSIGN(std::string v, location_type l) {
      return symbol_type(token::TOK_REASSIGN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REASSIGN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REASSIGN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RECURSIVE(std::string v, location_type l) {
      return symbol_type(token::TOK_RECURSIVE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RECURSIVE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RECURSIVE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REF_P(std::string v, location_type l) {
      return symbol_type(token::TOK_REF_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REF_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REF_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REFERENCES(std::string v, location_type l) {
      return symbol_type(token::TOK_REFERENCES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REFERENCES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REFERENCES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REFERENCING(std::string v, location_type l) {
      return symbol_type(token::TOK_REFERENCING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REFERENCING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REFERENCING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REFRESH(std::string v, location_type l) {
      return symbol_type(token::TOK_REFRESH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REFRESH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REFRESH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REINDEX(std::string v, location_type l) {
      return symbol_type(token::TOK_REINDEX, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REINDEX(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REINDEX, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RELATIVE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_RELATIVE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RELATIVE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RELATIVE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RELEASE(std::string v, location_type l) {
      return symbol_type(token::TOK_RELEASE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RELEASE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RELEASE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RENAME(std::string v, location_type l) {
      return symbol_type(token::TOK_RENAME, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RENAME(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RENAME, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REPEATABLE(std::string v, location_type l) {
      return symbol_type(token::TOK_REPEATABLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REPEATABLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REPEATABLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REPLACE(std::string v, location_type l) {
      return symbol_type(token::TOK_REPLACE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REPLACE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REPLACE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REPLICA(std::string v, location_type l) {
      return symbol_type(token::TOK_REPLICA, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REPLICA(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REPLICA, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RESET(std::string v, location_type l) {
      return symbol_type(token::TOK_RESET, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RESET(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RESET, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RESTART(std::string v, location_type l) {
      return symbol_type(token::TOK_RESTART, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RESTART(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RESTART, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RESTRICT(std::string v, location_type l) {
      return symbol_type(token::TOK_RESTRICT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RESTRICT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RESTRICT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RETURN(std::string v, location_type l) {
      return symbol_type(token::TOK_RETURN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RETURN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RETURN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RETURNING(std::string v, location_type l) {
      return symbol_type(token::TOK_RETURNING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RETURNING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RETURNING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RETURNS(std::string v, location_type l) {
      return symbol_type(token::TOK_RETURNS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RETURNS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RETURNS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_REVOKE(std::string v, location_type l) {
      return symbol_type(token::TOK_REVOKE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_REVOKE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_REVOKE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RIGHT(std::string v, location_type l) {
      return symbol_type(token::TOK_RIGHT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RIGHT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RIGHT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ROLE(std::string v, location_type l) {
      return symbol_type(token::TOK_ROLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ROLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ROLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ROLLBACK(std::string v, location_type l) {
      return symbol_type(token::TOK_ROLLBACK, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ROLLBACK(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ROLLBACK, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ROLLUP(std::string v, location_type l) {
      return symbol_type(token::TOK_ROLLUP, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ROLLUP(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ROLLUP, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ROUTINE(std::string v, location_type l) {
      return symbol_type(token::TOK_ROUTINE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ROUTINE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ROUTINE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ROUTINES(std::string v, location_type l) {
      return symbol_type(token::TOK_ROUTINES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ROUTINES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ROUTINES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ROW(std::string v, location_type l) {
      return symbol_type(token::TOK_ROW, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ROW(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ROW, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ROWS(std::string v, location_type l) {
      return symbol_type(token::TOK_ROWS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ROWS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ROWS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_RULE(std::string v, location_type l) {
      return symbol_type(token::TOK_RULE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_RULE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_RULE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SAVEPOINT(std::string v, location_type l) {
      return symbol_type(token::TOK_SAVEPOINT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SAVEPOINT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SAVEPOINT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SCALAR(std::string v, location_type l) {
      return symbol_type(token::TOK_SCALAR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SCALAR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SCALAR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SCHEMA(std::string v, location_type l) {
      return symbol_type(token::TOK_SCHEMA, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SCHEMA(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SCHEMA, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SCHEMAS(std::string v, location_type l) {
      return symbol_type(token::TOK_SCHEMAS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SCHEMAS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SCHEMAS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SCROLL(std::string v, location_type l) {
      return symbol_type(token::TOK_SCROLL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SCROLL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SCROLL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SEARCH(std::string v, location_type l) {
      return symbol_type(token::TOK_SEARCH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SEARCH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SEARCH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SECOND_P(std::string v, location_type l) {
      return symbol_type(token::TOK_SECOND_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SECOND_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SECOND_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SECURITY(std::string v, location_type l) {
      return symbol_type(token::TOK_SECURITY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SECURITY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SECURITY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SELECT(std::string v, location_type l) {
      return symbol_type(token::TOK_SELECT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SELECT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SELECT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SEQUENCE(std::string v, location_type l) {
      return symbol_type(token::TOK_SEQUENCE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SEQUENCE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SEQUENCE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SEQUENCES(std::string v, location_type l) {
      return symbol_type(token::TOK_SEQUENCES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SEQUENCES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SEQUENCES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SERIALIZABLE(std::string v, location_type l) {
      return symbol_type(token::TOK_SERIALIZABLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SERIALIZABLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SERIALIZABLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SERVER(std::string v, location_type l) {
      return symbol_type(token::TOK_SERVER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SERVER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SERVER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SESSION(std::string v, location_type l) {
      return symbol_type(token::TOK_SESSION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SESSION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SESSION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SESSION_USER(std::string v, location_type l) {
      return symbol_type(token::TOK_SESSION_USER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SESSION_USER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SESSION_USER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SET(std::string v, location_type l) {
      return symbol_type(token::TOK_SET, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SET(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SET, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SETS(std::string v, location_type l) {
      return symbol_type(token::TOK_SETS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SETS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SETS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SETOF(std::string v, location_type l) {
      return symbol_type(token::TOK_SETOF, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SETOF(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SETOF, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SHARE(std::string v, location_type l) {
      return symbol_type(token::TOK_SHARE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SHARE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SHARE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SHOW(std::string v, location_type l) {
      return symbol_type(token::TOK_SHOW, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SHOW(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SHOW, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SIMILAR(std::string v, location_type l) {
      return symbol_type(token::TOK_SIMILAR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SIMILAR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SIMILAR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SIMPLE(std::string v, location_type l) {
      return symbol_type(token::TOK_SIMPLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SIMPLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SIMPLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SKIP(std::string v, location_type l) {
      return symbol_type(token::TOK_SKIP, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SKIP(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SKIP, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SMALLINT(std::string v, location_type l) {
      return symbol_type(token::TOK_SMALLINT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SMALLINT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SMALLINT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SNAPSHOT(std::string v, location_type l) {
      return symbol_type(token::TOK_SNAPSHOT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SNAPSHOT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SNAPSHOT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SOME(std::string v, location_type l) {
      return symbol_type(token::TOK_SOME, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SOME(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SOME, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SOURCE(std::string v, location_type l) {
      return symbol_type(token::TOK_SOURCE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SOURCE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SOURCE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SQL_P(std::string v, location_type l) {
      return symbol_type(token::TOK_SQL_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SQL_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SQL_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STABLE(std::string v, location_type l) {
      return symbol_type(token::TOK_STABLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STABLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STABLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STANDALONE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_STANDALONE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STANDALONE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STANDALONE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_START(std::string v, location_type l) {
      return symbol_type(token::TOK_START, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_START(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_START, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STATEMENT(std::string v, location_type l) {
      return symbol_type(token::TOK_STATEMENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STATEMENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STATEMENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STATISTICS(std::string v, location_type l) {
      return symbol_type(token::TOK_STATISTICS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STATISTICS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STATISTICS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STDIN(std::string v, location_type l) {
      return symbol_type(token::TOK_STDIN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STDIN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STDIN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STDOUT(std::string v, location_type l) {
      return symbol_type(token::TOK_STDOUT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STDOUT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STDOUT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STORAGE(std::string v, location_type l) {
      return symbol_type(token::TOK_STORAGE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STORAGE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STORAGE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STORED(std::string v, location_type l) {
      return symbol_type(token::TOK_STORED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STORED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STORED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STRICT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_STRICT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STRICT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STRICT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STRING_P(std::string v, location_type l) {
      return symbol_type(token::TOK_STRING_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STRING_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STRING_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_STRIP_P(std::string v, location_type l) {
      return symbol_type(token::TOK_STRIP_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_STRIP_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_STRIP_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SUBSCRIPTION(std::string v, location_type l) {
      return symbol_type(token::TOK_SUBSCRIPTION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SUBSCRIPTION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SUBSCRIPTION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SUBSTRING(std::string v, location_type l) {
      return symbol_type(token::TOK_SUBSTRING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SUBSTRING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SUBSTRING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SUPPORT(std::string v, location_type l) {
      return symbol_type(token::TOK_SUPPORT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SUPPORT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SUPPORT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SYMMETRIC(std::string v, location_type l) {
      return symbol_type(token::TOK_SYMMETRIC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SYMMETRIC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SYMMETRIC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SYSID(std::string v, location_type l) {
      return symbol_type(token::TOK_SYSID, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SYSID(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SYSID, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SYSTEM_P(std::string v, location_type l) {
      return symbol_type(token::TOK_SYSTEM_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SYSTEM_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SYSTEM_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_SYSTEM_USER(std::string v, location_type l) {
      return symbol_type(token::TOK_SYSTEM_USER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_SYSTEM_USER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_SYSTEM_USER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TABLE(std::string v, location_type l) {
      return symbol_type(token::TOK_TABLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TABLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TABLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TABLES(std::string v, location_type l) {
      return symbol_type(token::TOK_TABLES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TABLES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TABLES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TABLESAMPLE(std::string v, location_type l) {
      return symbol_type(token::TOK_TABLESAMPLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TABLESAMPLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TABLESAMPLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TABLESPACE(std::string v, location_type l) {
      return symbol_type(token::TOK_TABLESPACE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TABLESPACE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TABLESPACE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TARGET(std::string v, location_type l) {
      return symbol_type(token::TOK_TARGET, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TARGET(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TARGET, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TEMP(std::string v, location_type l) {
      return symbol_type(token::TOK_TEMP, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TEMP(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TEMP, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TEMPLATE(std::string v, location_type l) {
      return symbol_type(token::TOK_TEMPLATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TEMPLATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TEMPLATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TEMPORARY(std::string v, location_type l) {
      return symbol_type(token::TOK_TEMPORARY, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TEMPORARY(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TEMPORARY, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TEXT_P(std::string v, location_type l) {
      return symbol_type(token::TOK_TEXT_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TEXT_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TEXT_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_THEN(std::string v, location_type l) {
      return symbol_type(token::TOK_THEN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_THEN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_THEN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TIES(std::string v, location_type l) {
      return symbol_type(token::TOK_TIES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TIES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TIES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TIME(std::string v, location_type l) {
      return symbol_type(token::TOK_TIME, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TIME(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TIME, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TIMESTAMP(std::string v, location_type l) {
      return symbol_type(token::TOK_TIMESTAMP, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TIMESTAMP(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TIMESTAMP, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TO(std::string v, location_type l) {
      return symbol_type(token::TOK_TO, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TO(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TO, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TRAILING(std::string v, location_type l) {
      return symbol_type(token::TOK_TRAILING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TRAILING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TRAILING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TRANSACTION(std::string v, location_type l) {
      return symbol_type(token::TOK_TRANSACTION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TRANSACTION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TRANSACTION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TRANSFORM(std::string v, location_type l) {
      return symbol_type(token::TOK_TRANSFORM, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TRANSFORM(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TRANSFORM, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TREAT(std::string v, location_type l) {
      return symbol_type(token::TOK_TREAT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TREAT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TREAT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TRIGGER(std::string v, location_type l) {
      return symbol_type(token::TOK_TRIGGER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TRIGGER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TRIGGER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TRIM(std::string v, location_type l) {
      return symbol_type(token::TOK_TRIM, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TRIM(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TRIM, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TRUE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_TRUE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TRUE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TRUE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TRUNCATE(std::string v, location_type l) {
      return symbol_type(token::TOK_TRUNCATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TRUNCATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TRUNCATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TRUSTED(std::string v, location_type l) {
      return symbol_type(token::TOK_TRUSTED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TRUSTED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TRUSTED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TYPE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_TYPE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TYPE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TYPE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_TYPES_P(std::string v, location_type l) {
      return symbol_type(token::TOK_TYPES_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_TYPES_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_TYPES_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UESCAPE(std::string v, location_type l) {
      return symbol_type(token::TOK_UESCAPE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UESCAPE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UESCAPE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNBOUNDED(std::string v, location_type l) {
      return symbol_type(token::TOK_UNBOUNDED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNBOUNDED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNBOUNDED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNCONDITIONAL(std::string v, location_type l) {
      return symbol_type(token::TOK_UNCONDITIONAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNCONDITIONAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNCONDITIONAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNCOMMITTED(std::string v, location_type l) {
      return symbol_type(token::TOK_UNCOMMITTED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNCOMMITTED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNCOMMITTED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNENCRYPTED(std::string v, location_type l) {
      return symbol_type(token::TOK_UNENCRYPTED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNENCRYPTED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNENCRYPTED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNION(std::string v, location_type l) {
      return symbol_type(token::TOK_UNION, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNION(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNION, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNIQUE(std::string v, location_type l) {
      return symbol_type(token::TOK_UNIQUE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNIQUE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNIQUE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNKNOWN(std::string v, location_type l) {
      return symbol_type(token::TOK_UNKNOWN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNKNOWN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNKNOWN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNLISTEN(std::string v, location_type l) {
      return symbol_type(token::TOK_UNLISTEN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNLISTEN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNLISTEN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNLOGGED(std::string v, location_type l) {
      return symbol_type(token::TOK_UNLOGGED, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNLOGGED(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNLOGGED, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UNTIL(std::string v, location_type l) {
      return symbol_type(token::TOK_UNTIL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UNTIL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UNTIL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UPDATE(std::string v, location_type l) {
      return symbol_type(token::TOK_UPDATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_UPDATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_UPDATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_USER(std::string v, location_type l) {
      return symbol_type(token::TOK_USER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_USER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_USER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_USING(std::string v, location_type l) {
      return symbol_type(token::TOK_USING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_USING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_USING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VACUUM(std::string v, location_type l) {
      return symbol_type(token::TOK_VACUUM, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VACUUM(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VACUUM, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VALID(std::string v, location_type l) {
      return symbol_type(token::TOK_VALID, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VALID(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VALID, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VALIDATE(std::string v, location_type l) {
      return symbol_type(token::TOK_VALIDATE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VALIDATE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VALIDATE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VALIDATOR(std::string v, location_type l) {
      return symbol_type(token::TOK_VALIDATOR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VALIDATOR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VALIDATOR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VALUE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_VALUE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VALUE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VALUE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VALUES(std::string v, location_type l) {
      return symbol_type(token::TOK_VALUES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VALUES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VALUES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VARCHAR(std::string v, location_type l) {
      return symbol_type(token::TOK_VARCHAR, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VARCHAR(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VARCHAR, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VARIADIC(std::string v, location_type l) {
      return symbol_type(token::TOK_VARIADIC, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VARIADIC(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VARIADIC, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VARYING(std::string v, location_type l) {
      return symbol_type(token::TOK_VARYING, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VARYING(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VARYING, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VERBOSE(std::string v, location_type l) {
      return symbol_type(token::TOK_VERBOSE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VERBOSE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VERBOSE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VERSION_P(std::string v, location_type l) {
      return symbol_type(token::TOK_VERSION_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VERSION_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VERSION_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VIEW(std::string v, location_type l) {
      return symbol_type(token::TOK_VIEW, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VIEW(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VIEW, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VIEWS(std::string v, location_type l) {
      return symbol_type(token::TOK_VIEWS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VIEWS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VIEWS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VIRTUAL(std::string v, location_type l) {
      return symbol_type(token::TOK_VIRTUAL, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VIRTUAL(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VIRTUAL, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_VOLATILE(std::string v, location_type l) {
      return symbol_type(token::TOK_VOLATILE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_VOLATILE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_VOLATILE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WHEN(std::string v, location_type l) {
      return symbol_type(token::TOK_WHEN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WHEN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WHEN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WHERE(std::string v, location_type l) {
      return symbol_type(token::TOK_WHERE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WHERE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WHERE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WHITESPACE_P(std::string v, location_type l) {
      return symbol_type(token::TOK_WHITESPACE_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WHITESPACE_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WHITESPACE_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WINDOW(std::string v, location_type l) {
      return symbol_type(token::TOK_WINDOW, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WINDOW(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WINDOW, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WITH(std::string v, location_type l) {
      return symbol_type(token::TOK_WITH, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WITH(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WITH, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WITHIN(std::string v, location_type l) {
      return symbol_type(token::TOK_WITHIN, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WITHIN(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WITHIN, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WITHOUT(std::string v, location_type l) {
      return symbol_type(token::TOK_WITHOUT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WITHOUT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WITHOUT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WORK(std::string v, location_type l) {
      return symbol_type(token::TOK_WORK, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WORK(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WORK, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WRAPPER(std::string v, location_type l) {
      return symbol_type(token::TOK_WRAPPER, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WRAPPER(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WRAPPER, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WRITE(std::string v, location_type l) {
      return symbol_type(token::TOK_WRITE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_WRITE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_WRITE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XML_P(std::string v, location_type l) {
      return symbol_type(token::TOK_XML_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XML_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XML_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLATTRIBUTES(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLATTRIBUTES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLATTRIBUTES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLATTRIBUTES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLCONCAT(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLCONCAT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLCONCAT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLCONCAT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLELEMENT(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLELEMENT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLELEMENT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLELEMENT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLEXISTS(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLEXISTS, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLEXISTS(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLEXISTS, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLFOREST(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLFOREST, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLFOREST(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLFOREST, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLNAMESPACES(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLNAMESPACES, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLNAMESPACES(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLNAMESPACES, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLPARSE(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLPARSE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLPARSE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLPARSE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLPI(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLPI, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLPI(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLPI, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLROOT(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLROOT, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLROOT(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLROOT, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLSERIALIZE(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLSERIALIZE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLSERIALIZE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLSERIALIZE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_XMLTABLE(std::string v, location_type l) {
      return symbol_type(token::TOK_XMLTABLE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_XMLTABLE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_XMLTABLE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_YEAR_P(std::string v, location_type l) {
      return symbol_type(token::TOK_YEAR_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_YEAR_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_YEAR_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_YES_P(std::string v, location_type l) {
      return symbol_type(token::TOK_YES_P, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_YES_P(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_YES_P, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ZONE(std::string v, location_type l) {
      return symbol_type(token::TOK_ZONE, std::move(v), std::move(l));
   }
#else
   static symbol_type
   make_ZONE(const std::string& v, const location_type& l) {
      return symbol_type(token::TOK_ZONE, v, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_FORMAT_LA(location_type l) {
      return symbol_type(token::TOK_FORMAT_LA, std::move(l));
   }
#else
   static symbol_type
   make_FORMAT_LA(const location_type& l) {
      return symbol_type(token::TOK_FORMAT_LA, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NOT_LA(location_type l) {
      return symbol_type(token::TOK_NOT_LA, std::move(l));
   }
#else
   static symbol_type
   make_NOT_LA(const location_type& l) {
      return symbol_type(token::TOK_NOT_LA, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_NULLS_LA(location_type l) {
      return symbol_type(token::TOK_NULLS_LA, std::move(l));
   }
#else
   static symbol_type
   make_NULLS_LA(const location_type& l) {
      return symbol_type(token::TOK_NULLS_LA, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WITH_LA(location_type l) {
      return symbol_type(token::TOK_WITH_LA, std::move(l));
   }
#else
   static symbol_type
   make_WITH_LA(const location_type& l) {
      return symbol_type(token::TOK_WITH_LA, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_WITHOUT_LA(location_type l) {
      return symbol_type(token::TOK_WITHOUT_LA, std::move(l));
   }
#else
   static symbol_type
   make_WITHOUT_LA(const location_type& l) {
      return symbol_type(token::TOK_WITHOUT_LA, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_AND_PRIORITY(location_type l) {
      return symbol_type(token::TOK_AND_PRIORITY, std::move(l));
   }
#else
   static symbol_type
   make_AND_PRIORITY(const location_type& l) {
      return symbol_type(token::TOK_AND_PRIORITY, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_OR_PRIORITY(location_type l) {
      return symbol_type(token::TOK_OR_PRIORITY, std::move(l));
   }
#else
   static symbol_type
   make_OR_PRIORITY(const location_type& l) {
      return symbol_type(token::TOK_OR_PRIORITY, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_QUAL_OP(location_type l) {
      return symbol_type(token::TOK_QUAL_OP, std::move(l));
   }
#else
   static symbol_type
   make_QUAL_OP(const location_type& l) {
      return symbol_type(token::TOK_QUAL_OP, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_UMINUS(location_type l) {
      return symbol_type(token::TOK_UMINUS, std::move(l));
   }
#else
   static symbol_type
   make_UMINUS(const location_type& l) {
      return symbol_type(token::TOK_UMINUS, l);
   }
#endif
#if 201103L <= YY_CPLUSPLUS
   static symbol_type
   make_ROLLUP_PRIORITY(location_type l) {
      return symbol_type(token::TOK_ROLLUP_PRIORITY, std::move(l));
   }
#else
   static symbol_type
   make_ROLLUP_PRIORITY(const location_type& l) {
      return symbol_type(token::TOK_ROLLUP_PRIORITY, l);
   }
#endif

   class context {
      public:
      context(const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead() const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token() const YY_NOEXCEPT { return yyla_.kind(); }
      const location_type& location() const YY_NOEXCEPT { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens(symbol_kind_type yyarg[], int yyargn) const;

      private:
      const parser& yyparser_;
      const symbol_type& yyla_;
   };

   private:
#if YY_CPLUSPLUS < 201103L
   /// Non copyable.
   parser(const parser&);
   /// Non copyable.
   parser& operator=(const parser&);
#endif

   /// Check the lookahead yytoken.
   /// \returns  true iff the token will be eventually shifted.
   bool yy_lac_check_(symbol_kind_type yytoken) const;
   /// Establish the initial context if no initial context currently exists.
   /// \returns  true iff the token will be eventually shifted.
   bool yy_lac_establish_(symbol_kind_type yytoken);
   /// Discard any previous initial lookahead context because of event.
   /// \param event  the event which caused the lookahead to be discarded.
   ///               Only used for debbuging output.
   void yy_lac_discard_(const char* event);

   /// Stored state numbers (used for stacks).
   typedef short state_type;

   /// The arguments of the error message.
   int yy_syntax_error_arguments_(const context& yyctx,
                                  symbol_kind_type yyarg[], int yyargn) const;

   /// Generate an error message.
   /// \param yyctx     the context in which the error occurred.
   virtual std::string yysyntax_error_(const context& yyctx) const;
   /// Compute post-reduction state.
   /// \param yystate   the current state
   /// \param yysym     the nonterminal to push on the stack
   static state_type yy_lr_goto_state_(state_type yystate, int yysym);

   /// Whether the given \c yypact_ value indicates a defaulted state.
   /// \param yyvalue   the value to check
   static bool yy_pact_value_is_default_(int yyvalue) YY_NOEXCEPT;

   /// Whether the given \c yytable_ value indicates a syntax error.
   /// \param yyvalue   the value to check
   static bool yy_table_value_is_error_(int yyvalue) YY_NOEXCEPT;

   static const short yypact_ninf_;
   static const short yytable_ninf_;

   /// Convert a scanner token kind \a t to a symbol kind.
   /// In theory \a t should be a token_kind_type, but character literals
   /// are valid, yet not members of the token_kind_type enum.
   static symbol_kind_type yytranslate_(int t) YY_NOEXCEPT;

   // Tables.
   // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   // STATE-NUM.
   static const short yypact_[];

   // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   // Performed when YYTABLE does not specify something else to do.  Zero
   // means the default is an error.
   static const short yydefact_[];

   // YYPGOTO[NTERM-NUM].
   static const short yypgoto_[];

   // YYDEFGOTO[NTERM-NUM].
   static const short yydefgoto_[];

   // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   // positive, shift that token.  If negative, reduce the rule whose
   // number is the opposite.  If YYTABLE_NINF, syntax error.
   static const short yytable_[];

   static const short yycheck_[];

   // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   // state STATE-NUM.
   static const short yystos_[];

   // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
   static const short yyr1_[];

   // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
   static const signed char yyr2_[];

#if YYDEBUG
   // YYRLINE[YYN] -- Source line where rule number YYN was defined.
   static const short yyrline_[];
   /// Report on the debug stream that the rule \a r is going to be reduced.
   virtual void yy_reduce_print_(int r) const;
   /// Print the state stack on the debug stream.
   virtual void yy_stack_print_() const;

   /// Debugging level.
   int yydebug_;
   /// Debug stream.
   std::ostream* yycdebug_;

   /// \brief Display a symbol kind, value and location.
   /// \param yyo    The output stream.
   /// \param yysym  The symbol.
   template <typename Base>
   void yy_print_(std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

   /// \brief Reclaim the memory associated to a symbol.
   /// \param yymsg     Why this token is reclaimed.
   ///                  If null, print nothing.
   /// \param yysym     The symbol.
   template <typename Base>
   void yy_destroy_(const char* yymsg, basic_symbol<Base>& yysym) const;

   private:
   /// Type access provider for state based symbols.
   struct by_state {
      /// Default constructor.
      by_state() YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state(kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state(const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear() YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move(by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind() const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
   };

   /// "Internal" symbol: element of the stack.
   struct stack_symbol_type : basic_symbol<by_state> {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type();
      /// Move or copy construction.
      stack_symbol_type(YY_RVREF(stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type(state_type s, YY_MOVE_REF(symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator=(stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator=(const stack_symbol_type& that);
#endif
   };

   /// A stack with random access from its top.
   template <typename T, typename S = std::vector<T>>
   class stack {
      public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack(size_type n = 200) YY_NOEXCEPT
         : seq_(n) {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack(const stack&) = delete;
      /// Non copyable.
      stack& operator=(const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[](index_type i) const {
         return seq_[size_type(size() - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[](index_type i) {
         return seq_[size_type(size() - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push(YY_MOVE_REF(T) t) {
         seq_.push_back(T());
         operator[](0).move(t);
      }

      /// Pop elements from the stack.
      void
      pop(std::ptrdiff_t n = 1) YY_NOEXCEPT {
         for (; 0 < n; --n)
            seq_.pop_back();
      }

      /// Pop all elements from the stack.
      void
      clear() YY_NOEXCEPT {
         seq_.clear();
      }

      /// Number of elements on the stack.
      index_type
      size() const YY_NOEXCEPT {
         return index_type(seq_.size());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin() const YY_NOEXCEPT {
         return seq_.begin();
      }

      /// Bottom of the stack.
      const_iterator
      end() const YY_NOEXCEPT {
         return seq_.end();
      }

      /// Present a slice of the top of a stack.
      class slice {
         public:
         slice(const stack& stack, index_type range) YY_NOEXCEPT
            : stack_(stack),
              range_(range) {}

         const T&
         operator[](index_type i) const {
            return stack_[range_ - i];
         }

         private:
         const stack& stack_;
         index_type range_;
      };

      private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack(const stack&);
      /// Non copyable.
      stack& operator=(const stack&);
#endif
      /// The wrapped container.
      S seq_;
   };

   /// Stack type.
   typedef stack<stack_symbol_type> stack_type;

   /// The stack.
   stack_type yystack_;
   /// The stack for LAC.
   /// Logically, the yy_lac_stack's lifetime is confined to the function
   /// yy_lac_check_. We just store it as a member of this class to hold
   /// on to the memory and to avoid frequent reallocations.
   /// Since yy_lac_check_ is const, this member must be mutable.
   mutable std::vector<state_type> yylac_stack_;
   /// Whether an initial LAC context was established.
   bool yy_lac_established_;

   /// Push a new state on the stack.
   /// \param m    a debug message to display
   ///             if null, no trace is output.
   /// \param sym  the symbol
   /// \warning the contents of \a s.value is stolen.
   void yypush_(const char* m, YY_MOVE_REF(stack_symbol_type) sym);

   /// Push a new look ahead token on the state on the stack.
   /// \param m    a debug message to display
   ///             if null, no trace is output.
   /// \param s    the state
   /// \param sym  the symbol (for its value and location).
   /// \warning the contents of \a sym.value is stolen.
   void yypush_(const char* m, state_type s, YY_MOVE_REF(symbol_type) sym);

   /// Pop \a n symbols from the stack.
   void yypop_(int n = 1) YY_NOEXCEPT;

   /// Constants.
   enum {
      yylast_ = 21418, ///< Last index in yytable_.
      yynnts_ = 187, ///< Number of nonterminal symbols.
      yyfinal_ = 463 ///< Termination state number.
   };

   // User arguments.
   Driver& drv;
};

inline parser::symbol_kind_type
parser::yytranslate_(int t) YY_NOEXCEPT {
   return static_cast<symbol_kind_type>(t);
}

// basic_symbol.
template <typename Base>
parser::basic_symbol<Base>::basic_symbol(const basic_symbol& that)
   : Base(that), value(), location(that.location) {
   switch (this->kind()) {
      case symbol_kind::S_distinct_clause: // distinct_clause
      case symbol_kind::S_set_quantifier: // set_quantifier
      case symbol_kind::S_opt_asymmetric: // opt_asymmetric
      case symbol_kind::S_OptTemp: // OptTemp
      case symbol_kind::S_opt_varying: // opt_varying
         value.copy<bool>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_ICONST: // ICONST
         value.copy<int>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_when_clause: // when_clause
         value.copy<lingodb::ast::CaseExpression::CaseCheck>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_basicComparisonType: // basicComparisonType
         value.copy<lingodb::ast::ExpressionType>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_join_type: // join_type
         value.copy<lingodb::ast::JoinType>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_Type: // Type
      case symbol_kind::S_SimpleType: // SimpleType
      case symbol_kind::S_ConstTypename: // ConstTypename
      case symbol_kind::S_Numeric_with_opt_lenghth: // Numeric_with_opt_lenghth
      case symbol_kind::S_Numeric: // Numeric
      case symbol_kind::S_Character: // Character
      case symbol_kind::S_ConstCharacter: // ConstCharacter
      case symbol_kind::S_CharacterWithLength: // CharacterWithLength
      case symbol_kind::S_CharacterWithoutLength: // CharacterWithoutLength
      case symbol_kind::S_character: // character
      case symbol_kind::S_ConstDatetime: // ConstDatetime
      case symbol_kind::S_ConstInterval: // ConstInterval
      case symbol_kind::S_Typename: // Typename
         value.copy<lingodb::ast::LogicalTypeWithMods>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_nulls_order: // opt_nulls_order
         value.copy<lingodb::ast::OrderByNullType>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_asc_desc: // opt_asc_desc
         value.copy<lingodb::ast::OrderType>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_sub_type: // sub_type
         value.copy<lingodb::ast::SubqueryType>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_join_qual: // join_qual
         value.copy<lingodb::ast::jointCondOrUsingCols>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_interval: // opt_interval
         value.copy<std::optional<lingodb::ast::SQLAbstractLogicalType>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_sort_clause: // opt_sort_clause
         value.copy<std::optional<std::shared_ptr<lingodb::ast::OrderByModifier>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_case_arg: // case_arg
         value.copy<std::optional<std::shared_ptr<lingodb::ast::ParsedExpression>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_select_limit: // opt_select_limit
         value.copy<std::optional<std::shared_ptr<lingodb::ast::ResultModifier>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_copy_opt_item: // copy_opt_item
         value.copy<std::pair<std::string, std::string>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_alias_clause: // alias_clause
      case symbol_kind::S_opt_alias_clause: // opt_alias_clause
         value.copy<std::pair<std::string, std::vector<std::string>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_extract_arg: // extract_arg
         value.copy<std::shared_ptr<ast::ConstantExpression>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_CopyStmt: // CopyStmt
         value.copy<std::shared_ptr<ast::CopyNode>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_VariableSetStmt: // VariableSetStmt
      case symbol_kind::S_set_rest: // set_rest
      case symbol_kind::S_generic_set: // generic_set
      case symbol_kind::S_set_rest_more: // set_rest_more
         value.copy<std::shared_ptr<ast::SetVariableStatement>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_agg_expr: // agg_expr
         value.copy<std::shared_ptr<lingodb::ast::AggregationNode>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_toplevel_stmt: // toplevel_stmt
      case symbol_kind::S_stmt: // stmt
         value.copy<std::shared_ptr<lingodb::ast::AstNode>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_and_a_expr: // and_a_expr
      case symbol_kind::S_or_a_expr: // or_a_expr
         value.copy<std::shared_ptr<lingodb::ast::ConjunctionExpression>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_ColConstraint: // ColConstraint
      case symbol_kind::S_ColConstraintElem: // ColConstraintElem
      case symbol_kind::S_ConstraintElem: // ConstraintElem
         value.copy<std::shared_ptr<lingodb::ast::Constraint>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_CreateStmt: // CreateStmt
         value.copy<std::shared_ptr<lingodb::ast::CreateNode>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_from_list: // from_list
         value.copy<std::shared_ptr<lingodb::ast::CrossProductRef>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_values_clause: // values_clause
         value.copy<std::shared_ptr<lingodb::ast::ExpressionListRef>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_func_application: // func_application
      case symbol_kind::S_func_expr: // func_expr
      case symbol_kind::S_func_expr_common_subexpr: // func_expr_common_subexpr
         value.copy<std::shared_ptr<lingodb::ast::FunctionExpression>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_group_clause: // group_clause
      case symbol_kind::S_group_clause_with_alias: // group_clause_with_alias
         value.copy<std::shared_ptr<lingodb::ast::GroupByNode>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_InsertStmt: // InsertStmt
      case symbol_kind::S_insert_rest: // insert_rest
         value.copy<std::shared_ptr<lingodb::ast::InsertNode>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_select_limit: // select_limit
      case symbol_kind::S_limit_clause: // limit_clause
         value.copy<std::shared_ptr<lingodb::ast::LimitModifier>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_sortby: // sortby
         value.copy<std::shared_ptr<lingodb::ast::OrderByElement>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_sort_clause: // sort_clause
         value.copy<std::shared_ptr<lingodb::ast::OrderByModifier>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_var_value: // var_value
      case symbol_kind::S_opt_boolean_or_string: // opt_boolean_or_string
      case symbol_kind::S_NonReservedWord_or_Sconst: // NonReservedWord_or_Sconst
      case symbol_kind::S_group_by_item: // group_by_item
      case symbol_kind::S_group_by_item_with_alias: // group_by_item_with_alias
      case symbol_kind::S_having_clause: // having_clause
      case symbol_kind::S_offset_clause: // offset_clause
      case symbol_kind::S_select_limit_value: // select_limit_value
      case symbol_kind::S_select_offset_value: // select_offset_value
      case symbol_kind::S_where_clause: // where_clause
      case symbol_kind::S_a_expr: // a_expr
      case symbol_kind::S_b_expr: // b_expr
      case symbol_kind::S_c_expr: // c_expr
      case symbol_kind::S_case_expr: // case_expr
      case symbol_kind::S_case_default: // case_default
      case symbol_kind::S_columnref: // columnref
      case symbol_kind::S_func_arg_expr: // func_arg_expr
      case symbol_kind::S_cast_expr: // cast_expr
      case symbol_kind::S_indirection: // indirection
      case symbol_kind::S_indirection_el: // indirection_el
      case symbol_kind::S_target_el: // target_el
      case symbol_kind::S_NumericOnly: // NumericOnly
      case symbol_kind::S_AexprConst: // AexprConst
      case symbol_kind::S_Iconst: // Iconst
      case symbol_kind::S_SignedIconst: // SignedIconst
      case symbol_kind::S_Fconst: // Fconst
      case symbol_kind::S_Sconst: // Sconst
      case symbol_kind::S_Bconst: // Bconst
         value.copy<std::shared_ptr<lingodb::ast::ParsedExpression>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_pipe_start: // pipe_start
      case symbol_kind::S_pipe_operator: // pipe_operator
         value.copy<std::shared_ptr<lingodb::ast::PipeOperator>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_select_with_parens: // select_with_parens
      case symbol_kind::S_select_no_parens: // select_no_parens
      case symbol_kind::S_select_clause: // select_clause
      case symbol_kind::S_simple_select: // simple_select
      case symbol_kind::S_with_clause: // with_clause
      case symbol_kind::S_cte_list: // cte_list
      case symbol_kind::S_common_table_expr: // common_table_expr
         value.copy<std::shared_ptr<lingodb::ast::QueryNode>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_set_list: // set_list
         value.copy<std::shared_ptr<lingodb::ast::SetColumnExpression>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_TableElement: // TableElement
      case symbol_kind::S_TableConstraint: // TableConstraint
      case symbol_kind::S_columnElement: // columnElement
         value.copy<std::shared_ptr<lingodb::ast::TableElement>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_SelectStmt: // SelectStmt
      case symbol_kind::S_PipeSQLStmt: // PipeSQLStmt
      case symbol_kind::S_classic_select_and_pipe_sql_with_parens: // classic_select_and_pipe_sql_with_parens
      case symbol_kind::S_pipe_sql_with_parens: // pipe_sql_with_parens
      case symbol_kind::S_pipe_sql_no_parens: // pipe_sql_no_parens
      case symbol_kind::S_pipe_or_select_clause: // pipe_or_select_clause
      case symbol_kind::S_PreparableStmt: // PreparableStmt
         value.copy<std::shared_ptr<lingodb::ast::TableProducer>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_from_clause: // opt_from_clause
      case symbol_kind::S_from_clause: // from_clause
      case symbol_kind::S_table_ref: // table_ref
      case symbol_kind::S_joined_table: // joined_table
         value.copy<std::shared_ptr<lingodb::ast::TableRef>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_target_list: // opt_target_list
         value.copy<std::shared_ptr<lingodb::ast::TargetList>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_type_modifier: // type_modifier
         value.copy<std::shared_ptr<lingodb::ast::Value>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_window_func_expr: // window_func_expr
      case symbol_kind::S_over_clause: // over_clause
      case symbol_kind::S_window_specification: // window_specification
         value.copy<std::shared_ptr<lingodb::ast::WindowExpression>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_frame_clause: // opt_frame_clause
      case symbol_kind::S_frame_extent: // frame_extent
      case symbol_kind::S_frame_bound: // frame_bound
         value.copy<std::shared_ptr<lingodb::ast::WindowFrame>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_FCONST: // FCONST
      case symbol_kind::S_IDENT: // IDENT
      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_STRING_VALUE: // STRING_VALUE
      case symbol_kind::S_BIT_VALUE: // "bit_string"
      case symbol_kind::S_HEX_VALUE: // "hex_string"
      case symbol_kind::S_NATIONAL_VALUE: // "nat_string"
      case symbol_kind::S_PERCENT: // "%"
      case symbol_kind::S_PLUS: // "+"
      case symbol_kind::S_SLASH: // "/"
      case symbol_kind::S_STAR: // "*"
      case symbol_kind::S_MINUS: // "-"
      case symbol_kind::S_EQUAL: // "="
      case symbol_kind::S_NOT_EQUAL: // "<>"
      case symbol_kind::S_LESS_EQUAL: // "<="
      case symbol_kind::S_LESS: // "<"
      case symbol_kind::S_GREATER_EQUAL: // ">="
      case symbol_kind::S_GREATER: // ">"
      case symbol_kind::S_UIDENT: // UIDENT
      case symbol_kind::S_SCONST: // SCONST
      case symbol_kind::S_USCONST: // USCONST
      case symbol_kind::S_BCONST: // BCONST
      case symbol_kind::S_XCONST: // XCONST
      case symbol_kind::S_Op: // Op
      case symbol_kind::S_TYPECAST: // TYPECAST
      case symbol_kind::S_ABORT_P: // ABORT_P
      case symbol_kind::S_ABSENT: // ABSENT
      case symbol_kind::S_ABSOLUTE_P: // ABSOLUTE_P
      case symbol_kind::S_ACCESS: // ACCESS
      case symbol_kind::S_ACTION: // ACTION
      case symbol_kind::S_ADD_P: // ADD_P
      case symbol_kind::S_ADMIN: // ADMIN
      case symbol_kind::S_AFTER: // AFTER
      case symbol_kind::S_AGGREGATE: // AGGREGATE
      case symbol_kind::S_ALL: // ALL
      case symbol_kind::S_ALSO: // ALSO
      case symbol_kind::S_ALTER: // ALTER
      case symbol_kind::S_ALWAYS: // ALWAYS
      case symbol_kind::S_ANALYSE: // ANALYSE
      case symbol_kind::S_ANALYZE: // ANALYZE
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ANY: // ANY
      case symbol_kind::S_ARRAY: // ARRAY
      case symbol_kind::S_AS: // AS
      case symbol_kind::S_ASC: // ASC
      case symbol_kind::S_ASENSITIVE: // ASENSITIVE
      case symbol_kind::S_ASSERTION: // ASSERTION
      case symbol_kind::S_ASSIGNMENT: // ASSIGNMENT
      case symbol_kind::S_ASYMMETRIC: // ASYMMETRIC
      case symbol_kind::S_ATOMIC: // ATOMIC
      case symbol_kind::S_AT: // AT
      case symbol_kind::S_ATTACH: // ATTACH
      case symbol_kind::S_ATTRIBUTE: // ATTRIBUTE
      case symbol_kind::S_AUTHORIZATION: // AUTHORIZATION
      case symbol_kind::S_BACKWARD: // BACKWARD
      case symbol_kind::S_BEFORE: // BEFORE
      case symbol_kind::S_BEGIN_P: // BEGIN_P
      case symbol_kind::S_BETWEEN: // BETWEEN
      case symbol_kind::S_BIGINT: // BIGINT
      case symbol_kind::S_BINARY: // BINARY
      case symbol_kind::S_BIT: // BIT
      case symbol_kind::S_BOOLEAN_P: // BOOLEAN_P
      case symbol_kind::S_BOTH: // BOTH
      case symbol_kind::S_BREADTH: // BREADTH
      case symbol_kind::S_BY: // BY
      case symbol_kind::S_CACHE: // CACHE
      case symbol_kind::S_CALL: // CALL
      case symbol_kind::S_CALLED: // CALLED
      case symbol_kind::S_CASCADE: // CASCADE
      case symbol_kind::S_CASCADED: // CASCADED
      case symbol_kind::S_CASE: // CASE
      case symbol_kind::S_CAST: // CAST
      case symbol_kind::S_CATALOG_P: // CATALOG_P
      case symbol_kind::S_CHAIN: // CHAIN
      case symbol_kind::S_CHAR_P: // CHAR_P
      case symbol_kind::S_CHARACTER: // CHARACTER
      case symbol_kind::S_CHARACTERISTICS: // CHARACTERISTICS
      case symbol_kind::S_CHECK: // CHECK
      case symbol_kind::S_CHECKPOINT: // CHECKPOINT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_CLOSE: // CLOSE
      case symbol_kind::S_CLUSTER: // CLUSTER
      case symbol_kind::S_COALESCE: // COALESCE
      case symbol_kind::S_COLLATE: // COLLATE
      case symbol_kind::S_COLLATION: // COLLATION
      case symbol_kind::S_COLUMN: // COLUMN
      case symbol_kind::S_COLUMNS: // COLUMNS
      case symbol_kind::S_COMMENT: // COMMENT
      case symbol_kind::S_COMMENTS: // COMMENTS
      case symbol_kind::S_COMMIT: // COMMIT
      case symbol_kind::S_COMMITTED: // COMMITTED
      case symbol_kind::S_COMPRESSION: // COMPRESSION
      case symbol_kind::S_CONCURRENTLY: // CONCURRENTLY
      case symbol_kind::S_CONDITIONAL: // CONDITIONAL
      case symbol_kind::S_CONFIGURATION: // CONFIGURATION
      case symbol_kind::S_CONFLICT: // CONFLICT
      case symbol_kind::S_CONNECTION: // CONNECTION
      case symbol_kind::S_CONSTRAINT: // CONSTRAINT
      case symbol_kind::S_CONSTRAINTS: // CONSTRAINTS
      case symbol_kind::S_CONTENT_P: // CONTENT_P
      case symbol_kind::S_CONTINUE_P: // CONTINUE_P
      case symbol_kind::S_CONVERSION_P: // CONVERSION_P
      case symbol_kind::S_COPY: // COPY
      case symbol_kind::S_COST: // COST
      case symbol_kind::S_CREATE: // CREATE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_CSV: // CSV
      case symbol_kind::S_CUBE: // CUBE
      case symbol_kind::S_CURRENT_P: // CURRENT_P
      case symbol_kind::S_CURRENT_CATALOG: // CURRENT_CATALOG
      case symbol_kind::S_CURRENT_DATE: // CURRENT_DATE
      case symbol_kind::S_CURRENT_ROLE: // CURRENT_ROLE
      case symbol_kind::S_CURRENT_SCHEMA: // CURRENT_SCHEMA
      case symbol_kind::S_CURRENT_TIME: // CURRENT_TIME
      case symbol_kind::S_CURRENT_TIMESTAMP: // CURRENT_TIMESTAMP
      case symbol_kind::S_CURRENT_USER: // CURRENT_USER
      case symbol_kind::S_CURSOR: // CURSOR
      case symbol_kind::S_CYCLE: // CYCLE
      case symbol_kind::S_DATA_P: // DATA_P
      case symbol_kind::S_DATABASE: // DATABASE
      case symbol_kind::S_DAY_P: // DAY_P
      case symbol_kind::S_DEALLOCATE: // DEALLOCATE
      case symbol_kind::S_DEC: // DEC
      case symbol_kind::S_DECIMAL_P: // DECIMAL_P
      case symbol_kind::S_DECLARE: // DECLARE
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_DEFAULTS: // DEFAULTS
      case symbol_kind::S_DATE_P: // DATE_P
      case symbol_kind::S_DEFERRABLE: // DEFERRABLE
      case symbol_kind::S_DEFERRED: // DEFERRED
      case symbol_kind::S_DEFINER: // DEFINER
      case symbol_kind::S_DELETE_P: // DELETE_P
      case symbol_kind::S_DELIMITER: // DELIMITER
      case symbol_kind::S_DELIMITERS: // DELIMITERS
      case symbol_kind::S_DEPENDS: // DEPENDS
      case symbol_kind::S_DEPTH: // DEPTH
      case symbol_kind::S_DESC: // DESC
      case symbol_kind::S_DETACH: // DETACH
      case symbol_kind::S_DICTIONARY: // DICTIONARY
      case symbol_kind::S_DISABLE_P: // DISABLE_P
      case symbol_kind::S_DISCARD: // DISCARD
      case symbol_kind::S_DISTINCT: // DISTINCT
      case symbol_kind::S_DO: // DO
      case symbol_kind::S_DOCUMENT_P: // DOCUMENT_P
      case symbol_kind::S_DOMAIN_P: // DOMAIN_P
      case symbol_kind::S_DOUBLE_P: // DOUBLE_P
      case symbol_kind::S_DROP: // DROP
      case symbol_kind::S_EACH: // EACH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EMPTY_P: // EMPTY_P
      case symbol_kind::S_ENABLE_P: // ENABLE_P
      case symbol_kind::S_ENCODING: // ENCODING
      case symbol_kind::S_ENCRYPTED: // ENCRYPTED
      case symbol_kind::S_END_P: // END_P
      case symbol_kind::S_ENFORCED: // ENFORCED
      case symbol_kind::S_ENUM_P: // ENUM_P
      case symbol_kind::S_ERROR_P: // ERROR_P
      case symbol_kind::S_ESCAPE: // ESCAPE
      case symbol_kind::S_EVENT: // EVENT
      case symbol_kind::S_EXCEPT: // EXCEPT
      case symbol_kind::S_EXCLUDE: // EXCLUDE
      case symbol_kind::S_EXCLUDING: // EXCLUDING
      case symbol_kind::S_EXCLUSIVE: // EXCLUSIVE
      case symbol_kind::S_EXECUTE: // EXECUTE
      case symbol_kind::S_EXISTS: // EXISTS
      case symbol_kind::S_EXPLAIN: // EXPLAIN
      case symbol_kind::S_EXPRESSION: // EXPRESSION
      case symbol_kind::S_EXTENSION: // EXTENSION
      case symbol_kind::S_EXTERNAL: // EXTERNAL
      case symbol_kind::S_EXTRACT: // EXTRACT
      case symbol_kind::S_FALSE_P: // FALSE_P
      case symbol_kind::S_FAMILY: // FAMILY
      case symbol_kind::S_FETCH: // FETCH
      case symbol_kind::S_FILTER: // FILTER
      case symbol_kind::S_FINALIZE: // FINALIZE
      case symbol_kind::S_FIRST_P: // FIRST_P
      case symbol_kind::S_FLOAT_P: // FLOAT_P
      case symbol_kind::S_FOLLOWING: // FOLLOWING
      case symbol_kind::S_FOR: // FOR
      case symbol_kind::S_FORCE: // FORCE
      case symbol_kind::S_FOREIGN: // FOREIGN
      case symbol_kind::S_FORMAT: // FORMAT
      case symbol_kind::S_FORWARD: // FORWARD
      case symbol_kind::S_FREEZE: // FREEZE
      case symbol_kind::S_FROM: // FROM
      case symbol_kind::S_FULL: // FULL
      case symbol_kind::S_FUNCTION: // FUNCTION
      case symbol_kind::S_FUNCTIONS: // FUNCTIONS
      case symbol_kind::S_GENERATED: // GENERATED
      case symbol_kind::S_GLOBAL: // GLOBAL
      case symbol_kind::S_GRANT: // GRANT
      case symbol_kind::S_GRANTED: // GRANTED
      case symbol_kind::S_GREATEST: // GREATEST
      case symbol_kind::S_GROUP_P: // GROUP_P
      case symbol_kind::S_GROUPING: // GROUPING
      case symbol_kind::S_GROUPS: // GROUPS
      case symbol_kind::S_HANDLER: // HANDLER
      case symbol_kind::S_HAVING: // HAVING
      case symbol_kind::S_HEADER_P: // HEADER_P
      case symbol_kind::S_HOLD: // HOLD
      case symbol_kind::S_HOUR_P: // HOUR_P
      case symbol_kind::S_IDENTITY_P: // IDENTITY_P
      case symbol_kind::S_IF_P: // IF_P
      case symbol_kind::S_ILIKE: // ILIKE
      case symbol_kind::S_IMMEDIATE: // IMMEDIATE
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_IMPLICIT_P: // IMPLICIT_P
      case symbol_kind::S_IMPORT_P: // IMPORT_P
      case symbol_kind::S_IN_P: // IN_P
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_INCLUDING: // INCLUDING
      case symbol_kind::S_INCREMENT: // INCREMENT
      case symbol_kind::S_INDENT: // INDENT
      case symbol_kind::S_INDEX: // INDEX
      case symbol_kind::S_INDEXES: // INDEXES
      case symbol_kind::S_INHERIT: // INHERIT
      case symbol_kind::S_INHERITS: // INHERITS
      case symbol_kind::S_INITIALLY: // INITIALLY
      case symbol_kind::S_INLINE_P: // INLINE_P
      case symbol_kind::S_INNER_P: // INNER_P
      case symbol_kind::S_INOUT: // INOUT
      case symbol_kind::S_INPUT_P: // INPUT_P
      case symbol_kind::S_INSENSITIVE: // INSENSITIVE
      case symbol_kind::S_INSERT: // INSERT
      case symbol_kind::S_INSTEAD: // INSTEAD
      case symbol_kind::S_INT_P: // INT_P
      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_INTERSECT: // INTERSECT
      case symbol_kind::S_INTERVAL: // INTERVAL
      case symbol_kind::S_INTO: // INTO
      case symbol_kind::S_INVOKER: // INVOKER
      case symbol_kind::S_IS: // IS
      case symbol_kind::S_ISNULL: // ISNULL
      case symbol_kind::S_ISOLATION: // ISOLATION
      case symbol_kind::S_JOIN: // JOIN
      case symbol_kind::S_JSON: // JSON
      case symbol_kind::S_JSON_ARRAY: // JSON_ARRAY
      case symbol_kind::S_JSON_ARRAYAGG: // JSON_ARRAYAGG
      case symbol_kind::S_JSON_EXISTS: // JSON_EXISTS
      case symbol_kind::S_JSON_OBJECT: // JSON_OBJECT
      case symbol_kind::S_JSON_OBJECTAGG: // JSON_OBJECTAGG
      case symbol_kind::S_JSON_QUERY: // JSON_QUERY
      case symbol_kind::S_JSON_SCALAR: // JSON_SCALAR
      case symbol_kind::S_JSON_SERIALIZE: // JSON_SERIALIZE
      case symbol_kind::S_JSON_TABLE: // JSON_TABLE
      case symbol_kind::S_JSON_VALUE: // JSON_VALUE
      case symbol_kind::S_KEEP: // KEEP
      case symbol_kind::S_KEY: // KEY
      case symbol_kind::S_KEYS: // KEYS
      case symbol_kind::S_LABEL: // LABEL
      case symbol_kind::S_LANGUAGE: // LANGUAGE
      case symbol_kind::S_LARGE_P: // LARGE_P
      case symbol_kind::S_LAST_P: // LAST_P
      case symbol_kind::S_LATERAL_P: // LATERAL_P
      case symbol_kind::S_LEADING: // LEADING
      case symbol_kind::S_LEAKPROOF: // LEAKPROOF
      case symbol_kind::S_LEAST: // LEAST
      case symbol_kind::S_LEFT: // LEFT
      case symbol_kind::S_LEVEL: // LEVEL
      case symbol_kind::S_LIKE: // LIKE
      case symbol_kind::S_LIMIT: // LIMIT
      case symbol_kind::S_LISTEN: // LISTEN
      case symbol_kind::S_LOAD: // LOAD
      case symbol_kind::S_LOCAL: // LOCAL
      case symbol_kind::S_LOCALTIME: // LOCALTIME
      case symbol_kind::S_LOCALTIMESTAMP: // LOCALTIMESTAMP
      case symbol_kind::S_LOCATION: // LOCATION
      case symbol_kind::S_LOCK_P: // LOCK_P
      case symbol_kind::S_LOCKED: // LOCKED
      case symbol_kind::S_LOGGED: // LOGGED
      case symbol_kind::S_MAPPING: // MAPPING
      case symbol_kind::S_MATCH: // MATCH
      case symbol_kind::S_MATCHED: // MATCHED
      case symbol_kind::S_MATERIALIZED: // MATERIALIZED
      case symbol_kind::S_MAXVALUE: // MAXVALUE
      case symbol_kind::S_MERGE: // MERGE
      case symbol_kind::S_MERGE_ACTION: // MERGE_ACTION
      case symbol_kind::S_METHOD: // METHOD
      case symbol_kind::S_MINUTE_P: // MINUTE_P
      case symbol_kind::S_MINVALUE: // MINVALUE
      case symbol_kind::S_MODE: // MODE
      case symbol_kind::S_MONTH_P: // MONTH_P
      case symbol_kind::S_MOVE: // MOVE
      case symbol_kind::S_NAME_P: // NAME_P
      case symbol_kind::S_NAMES: // NAMES
      case symbol_kind::S_NATIONAL: // NATIONAL
      case symbol_kind::S_NATURAL: // NATURAL
      case symbol_kind::S_NCHAR: // NCHAR
      case symbol_kind::S_NESTED: // NESTED
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_NEXT: // NEXT
      case symbol_kind::S_NFC: // NFC
      case symbol_kind::S_NFD: // NFD
      case symbol_kind::S_NFKC: // NFKC
      case symbol_kind::S_NFKD: // NFKD
      case symbol_kind::S_NO: // NO
      case symbol_kind::S_NONE: // NONE
      case symbol_kind::S_NORMALIZE: // NORMALIZE
      case symbol_kind::S_NORMALIZED: // NORMALIZED
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTHING: // NOTHING
      case symbol_kind::S_NOTIFY: // NOTIFY
      case symbol_kind::S_NOTNULL: // NOTNULL
      case symbol_kind::S_NOWAIT: // NOWAIT
      case symbol_kind::S_NULL_P: // NULL_P
      case symbol_kind::S_NULLIF: // NULLIF
      case symbol_kind::S_NULLS_P: // NULLS_P
      case symbol_kind::S_NUMERIC: // NUMERIC
      case symbol_kind::S_OBJECT_P: // OBJECT_P
      case symbol_kind::S_OBJECTS_P: // OBJECTS_P
      case symbol_kind::S_OF: // OF
      case symbol_kind::S_OFF: // OFF
      case symbol_kind::S_OFFSET: // OFFSET
      case symbol_kind::S_OIDS: // OIDS
      case symbol_kind::S_OLD: // OLD
      case symbol_kind::S_OMIT: // OMIT
      case symbol_kind::S_ON: // ON
      case symbol_kind::S_ONLY: // ONLY
      case symbol_kind::S_OPERATOR: // OPERATOR
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_OPTIONS: // OPTIONS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_ORDER: // ORDER
      case symbol_kind::S_ORDINALITY: // ORDINALITY
      case symbol_kind::S_OTHERS: // OTHERS
      case symbol_kind::S_OUT_P: // OUT_P
      case symbol_kind::S_OUTER_P: // OUTER_P
      case symbol_kind::S_OVER: // OVER
      case symbol_kind::S_OVERLAPS: // OVERLAPS
      case symbol_kind::S_OVERLAY: // OVERLAY
      case symbol_kind::S_OVERRIDING: // OVERRIDING
      case symbol_kind::S_OWNED: // OWNED
      case symbol_kind::S_OWNER: // OWNER
      case symbol_kind::S_PARALLEL: // PARALLEL
      case symbol_kind::S_PARAMETER: // PARAMETER
      case symbol_kind::S_PARSER: // PARSER
      case symbol_kind::S_PARTIAL: // PARTIAL
      case symbol_kind::S_PARTITION: // PARTITION
      case symbol_kind::S_PASSING: // PASSING
      case symbol_kind::S_PASSWORD: // PASSWORD
      case symbol_kind::S_PATH: // PATH
      case symbol_kind::S_PERIOD: // PERIOD
      case symbol_kind::S_PLACING: // PLACING
      case symbol_kind::S_PLAN: // PLAN
      case symbol_kind::S_PLANS: // PLANS
      case symbol_kind::S_POLICY: // POLICY
      case symbol_kind::S_POSITION: // POSITION
      case symbol_kind::S_PRECEDING: // PRECEDING
      case symbol_kind::S_PRECISION: // PRECISION
      case symbol_kind::S_PRESERVE: // PRESERVE
      case symbol_kind::S_PREPARE: // PREPARE
      case symbol_kind::S_PREPARED: // PREPARED
      case symbol_kind::S_PRIMARY: // PRIMARY
      case symbol_kind::S_PRIOR: // PRIOR
      case symbol_kind::S_PRIVILEGES: // PRIVILEGES
      case symbol_kind::S_PROCEDURAL: // PROCEDURAL
      case symbol_kind::S_PROCEDURE: // PROCEDURE
      case symbol_kind::S_PROCEDURES: // PROCEDURES
      case symbol_kind::S_PROGRAM: // PROGRAM
      case symbol_kind::S_PUBLICATION: // PUBLICATION
      case symbol_kind::S_QUOTES: // QUOTES
      case symbol_kind::S_EXTEND: // EXTEND
      case symbol_kind::S_RANGE: // RANGE
      case symbol_kind::S_READ: // READ
      case symbol_kind::S_REAL: // REAL
      case symbol_kind::S_REASSIGN: // REASSIGN
      case symbol_kind::S_RECURSIVE: // RECURSIVE
      case symbol_kind::S_REF_P: // REF_P
      case symbol_kind::S_REFERENCES: // REFERENCES
      case symbol_kind::S_REFERENCING: // REFERENCING
      case symbol_kind::S_REFRESH: // REFRESH
      case symbol_kind::S_REINDEX: // REINDEX
      case symbol_kind::S_RELATIVE_P: // RELATIVE_P
      case symbol_kind::S_RELEASE: // RELEASE
      case symbol_kind::S_RENAME: // RENAME
      case symbol_kind::S_REPEATABLE: // REPEATABLE
      case symbol_kind::S_REPLACE: // REPLACE
      case symbol_kind::S_REPLICA: // REPLICA
      case symbol_kind::S_RESET: // RESET
      case symbol_kind::S_RESTART: // RESTART
      case symbol_kind::S_RESTRICT: // RESTRICT
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RETURNING: // RETURNING
      case symbol_kind::S_RETURNS: // RETURNS
      case symbol_kind::S_REVOKE: // REVOKE
      case symbol_kind::S_RIGHT: // RIGHT
      case symbol_kind::S_ROLE: // ROLE
      case symbol_kind::S_ROLLBACK: // ROLLBACK
      case symbol_kind::S_ROLLUP: // ROLLUP
      case symbol_kind::S_ROUTINE: // ROUTINE
      case symbol_kind::S_ROUTINES: // ROUTINES
      case symbol_kind::S_ROW: // ROW
      case symbol_kind::S_ROWS: // ROWS
      case symbol_kind::S_RULE: // RULE
      case symbol_kind::S_SAVEPOINT: // SAVEPOINT
      case symbol_kind::S_SCALAR: // SCALAR
      case symbol_kind::S_SCHEMA: // SCHEMA
      case symbol_kind::S_SCHEMAS: // SCHEMAS
      case symbol_kind::S_SCROLL: // SCROLL
      case symbol_kind::S_SEARCH: // SEARCH
      case symbol_kind::S_SECOND_P: // SECOND_P
      case symbol_kind::S_SECURITY: // SECURITY
      case symbol_kind::S_SELECT: // SELECT
      case symbol_kind::S_SEQUENCE: // SEQUENCE
      case symbol_kind::S_SEQUENCES: // SEQUENCES
      case symbol_kind::S_SERIALIZABLE: // SERIALIZABLE
      case symbol_kind::S_SERVER: // SERVER
      case symbol_kind::S_SESSION: // SESSION
      case symbol_kind::S_SESSION_USER: // SESSION_USER
      case symbol_kind::S_SET: // SET
      case symbol_kind::S_SETS: // SETS
      case symbol_kind::S_SETOF: // SETOF
      case symbol_kind::S_SHARE: // SHARE
      case symbol_kind::S_SHOW: // SHOW
      case symbol_kind::S_SIMILAR: // SIMILAR
      case symbol_kind::S_SIMPLE: // SIMPLE
      case symbol_kind::S_SKIP: // SKIP
      case symbol_kind::S_SMALLINT: // SMALLINT
      case symbol_kind::S_SNAPSHOT: // SNAPSHOT
      case symbol_kind::S_SOME: // SOME
      case symbol_kind::S_SOURCE: // SOURCE
      case symbol_kind::S_SQL_P: // SQL_P
      case symbol_kind::S_STABLE: // STABLE
      case symbol_kind::S_STANDALONE_P: // STANDALONE_P
      case symbol_kind::S_START: // START
      case symbol_kind::S_STATEMENT: // STATEMENT
      case symbol_kind::S_STATISTICS: // STATISTICS
      case symbol_kind::S_STDIN: // STDIN
      case symbol_kind::S_STDOUT: // STDOUT
      case symbol_kind::S_STORAGE: // STORAGE
      case symbol_kind::S_STORED: // STORED
      case symbol_kind::S_STRICT_P: // STRICT_P
      case symbol_kind::S_STRING_P: // STRING_P
      case symbol_kind::S_STRIP_P: // STRIP_P
      case symbol_kind::S_SUBSCRIPTION: // SUBSCRIPTION
      case symbol_kind::S_SUBSTRING: // SUBSTRING
      case symbol_kind::S_SUPPORT: // SUPPORT
      case symbol_kind::S_SYMMETRIC: // SYMMETRIC
      case symbol_kind::S_SYSID: // SYSID
      case symbol_kind::S_SYSTEM_P: // SYSTEM_P
      case symbol_kind::S_SYSTEM_USER: // SYSTEM_USER
      case symbol_kind::S_TABLE: // TABLE
      case symbol_kind::S_TABLES: // TABLES
      case symbol_kind::S_TABLESAMPLE: // TABLESAMPLE
      case symbol_kind::S_TABLESPACE: // TABLESPACE
      case symbol_kind::S_TARGET: // TARGET
      case symbol_kind::S_TEMP: // TEMP
      case symbol_kind::S_TEMPLATE: // TEMPLATE
      case symbol_kind::S_TEMPORARY: // TEMPORARY
      case symbol_kind::S_TEXT_P: // TEXT_P
      case symbol_kind::S_THEN: // THEN
      case symbol_kind::S_TIES: // TIES
      case symbol_kind::S_TIME: // TIME
      case symbol_kind::S_TIMESTAMP: // TIMESTAMP
      case symbol_kind::S_TO: // TO
      case symbol_kind::S_TRAILING: // TRAILING
      case symbol_kind::S_TRANSACTION: // TRANSACTION
      case symbol_kind::S_TRANSFORM: // TRANSFORM
      case symbol_kind::S_TREAT: // TREAT
      case symbol_kind::S_TRIGGER: // TRIGGER
      case symbol_kind::S_TRIM: // TRIM
      case symbol_kind::S_TRUE_P: // TRUE_P
      case symbol_kind::S_TRUNCATE: // TRUNCATE
      case symbol_kind::S_TRUSTED: // TRUSTED
      case symbol_kind::S_TYPE_P: // TYPE_P
      case symbol_kind::S_TYPES_P: // TYPES_P
      case symbol_kind::S_UESCAPE: // UESCAPE
      case symbol_kind::S_UNBOUNDED: // UNBOUNDED
      case symbol_kind::S_UNCONDITIONAL: // UNCONDITIONAL
      case symbol_kind::S_UNCOMMITTED: // UNCOMMITTED
      case symbol_kind::S_UNENCRYPTED: // UNENCRYPTED
      case symbol_kind::S_UNION: // UNION
      case symbol_kind::S_UNIQUE: // UNIQUE
      case symbol_kind::S_UNKNOWN: // UNKNOWN
      case symbol_kind::S_UNLISTEN: // UNLISTEN
      case symbol_kind::S_UNLOGGED: // UNLOGGED
      case symbol_kind::S_UNTIL: // UNTIL
      case symbol_kind::S_UPDATE: // UPDATE
      case symbol_kind::S_USER: // USER
      case symbol_kind::S_USING: // USING
      case symbol_kind::S_VACUUM: // VACUUM
      case symbol_kind::S_VALID: // VALID
      case symbol_kind::S_VALIDATE: // VALIDATE
      case symbol_kind::S_VALIDATOR: // VALIDATOR
      case symbol_kind::S_VALUE_P: // VALUE_P
      case symbol_kind::S_VALUES: // VALUES
      case symbol_kind::S_VARCHAR: // VARCHAR
      case symbol_kind::S_VARIADIC: // VARIADIC
      case symbol_kind::S_VARYING: // VARYING
      case symbol_kind::S_VERBOSE: // VERBOSE
      case symbol_kind::S_VERSION_P: // VERSION_P
      case symbol_kind::S_VIEW: // VIEW
      case symbol_kind::S_VIEWS: // VIEWS
      case symbol_kind::S_VIRTUAL: // VIRTUAL
      case symbol_kind::S_VOLATILE: // VOLATILE
      case symbol_kind::S_WHEN: // WHEN
      case symbol_kind::S_WHERE: // WHERE
      case symbol_kind::S_WHITESPACE_P: // WHITESPACE_P
      case symbol_kind::S_WINDOW: // WINDOW
      case symbol_kind::S_WITH: // WITH
      case symbol_kind::S_WITHIN: // WITHIN
      case symbol_kind::S_WITHOUT: // WITHOUT
      case symbol_kind::S_WORK: // WORK
      case symbol_kind::S_WRAPPER: // WRAPPER
      case symbol_kind::S_WRITE: // WRITE
      case symbol_kind::S_XML_P: // XML_P
      case symbol_kind::S_XMLATTRIBUTES: // XMLATTRIBUTES
      case symbol_kind::S_XMLCONCAT: // XMLCONCAT
      case symbol_kind::S_XMLELEMENT: // XMLELEMENT
      case symbol_kind::S_XMLEXISTS: // XMLEXISTS
      case symbol_kind::S_XMLFOREST: // XMLFOREST
      case symbol_kind::S_XMLNAMESPACES: // XMLNAMESPACES
      case symbol_kind::S_XMLPARSE: // XMLPARSE
      case symbol_kind::S_XMLPI: // XMLPI
      case symbol_kind::S_XMLROOT: // XMLROOT
      case symbol_kind::S_XMLSERIALIZE: // XMLSERIALIZE
      case symbol_kind::S_XMLTABLE: // XMLTABLE
      case symbol_kind::S_YEAR_P: // YEAR_P
      case symbol_kind::S_YES_P: // YES_P
      case symbol_kind::S_ZONE: // ZONE
      case symbol_kind::S_copy_file_name: // copy_file_name
      case symbol_kind::S_copy_delimiter: // copy_delimiter
      case symbol_kind::S_var_name: // var_name
      case symbol_kind::S_NonReservedWord: // NonReservedWord
      case symbol_kind::S_relation_expr: // relation_expr
      case symbol_kind::S_any_operator: // any_operator
      case symbol_kind::S_qual_Op: // qual_Op
      case symbol_kind::S_all_Op: // all_Op
      case symbol_kind::S_ColId: // ColId
      case symbol_kind::S_type_function_name: // type_function_name
      case symbol_kind::S_type_func_name_keyword: // type_func_name_keyword
      case symbol_kind::S_ColLabel: // ColLabel
      case symbol_kind::S_BareColLabel: // BareColLabel
      case symbol_kind::S_reserved_keyword: // reserved_keyword
      case symbol_kind::S_unreserved_keyword: // unreserved_keyword
      case symbol_kind::S_col_name_keyword: // col_name_keyword
      case symbol_kind::S_qualified_name: // qualified_name
      case symbol_kind::S_name: // name
      case symbol_kind::S_attr_name: // attr_name
      case symbol_kind::S_func_name: // func_name
      case symbol_kind::S_insert_target: // insert_target
      case symbol_kind::S_insert_column_item: // insert_column_item
         value.copy<std::string>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_in_expr: // in_expr
         value.copy<std::variant<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>, std::shared_ptr<lingodb::ast::SubqueryExpression>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_when_clause_list: // when_clause_list
         value.copy<std::vector<lingodb::ast::CaseExpression::CaseCheck>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_copy_options: // copy_options
      case symbol_kind::S_copy_opt_list: // copy_opt_list
         value.copy<std::vector<std::pair<std::string, std::string>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_stmtmulti: // stmtmulti
         value.copy<std::vector<std::shared_ptr<lingodb::ast::AstNode>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_ColQualList: // ColQualList
         value.copy<std::vector<std::shared_ptr<lingodb::ast::Constraint>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_func_expr_list: // func_expr_list
         value.copy<std::vector<std::shared_ptr<lingodb::ast::FunctionExpression>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_sortby_list: // sortby_list
         value.copy<std::vector<std::shared_ptr<lingodb::ast::OrderByElement>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_var_list: // var_list
      case symbol_kind::S_group_by_list: // group_by_list
      case symbol_kind::S_group_by_list_with_alias: // group_by_list_with_alias
      case symbol_kind::S_rollup_clause: // rollup_clause
      case symbol_kind::S_columnref_list: // columnref_list
      case symbol_kind::S_func_arg_list_opt: // func_arg_list_opt
      case symbol_kind::S_func_arg_list: // func_arg_list
      case symbol_kind::S_expr_list: // expr_list
      case symbol_kind::S_expr_list_with_alias: // expr_list_with_alias
      case symbol_kind::S_extract_list: // extract_list
      case symbol_kind::S_substr_list: // substr_list
      case symbol_kind::S_target_list: // target_list
      case symbol_kind::S_opt_partition_clause: // opt_partition_clause
         value.copy<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_OptTableElementList: // OptTableElementList
      case symbol_kind::S_TableElementList: // TableElementList
         value.copy<std::vector<std::shared_ptr<lingodb::ast::TableElement>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_type_modifiers: // opt_type_modifiers
      case symbol_kind::S_type_modifiers: // type_modifiers
         value.copy<std::vector<std::shared_ptr<lingodb::ast::Value>>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_opt_name_list: // opt_name_list
      case symbol_kind::S_name_list: // name_list
      case symbol_kind::S_insert_column_list: // insert_column_list
         value.copy<std::vector<std::string>>(YY_MOVE(that.value));
         break;

      case symbol_kind::S_INTEGER_VALUE: // "integer_value"
         value.copy<uint64_t>(YY_MOVE(that.value));
         break;

      default:
         break;
   }
}

template <typename Base>
parser::symbol_kind_type
parser::basic_symbol<Base>::type_get() const YY_NOEXCEPT {
   return this->kind();
}

template <typename Base>
bool parser::basic_symbol<Base>::empty() const YY_NOEXCEPT {
   return this->kind() == symbol_kind::S_YYEMPTY;
}

template <typename Base>
void parser::basic_symbol<Base>::move(basic_symbol& s) {
   super_type::move(s);
   switch (this->kind()) {
      case symbol_kind::S_distinct_clause: // distinct_clause
      case symbol_kind::S_set_quantifier: // set_quantifier
      case symbol_kind::S_opt_asymmetric: // opt_asymmetric
      case symbol_kind::S_OptTemp: // OptTemp
      case symbol_kind::S_opt_varying: // opt_varying
         value.move<bool>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_ICONST: // ICONST
         value.move<int>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_when_clause: // when_clause
         value.move<lingodb::ast::CaseExpression::CaseCheck>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_basicComparisonType: // basicComparisonType
         value.move<lingodb::ast::ExpressionType>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_join_type: // join_type
         value.move<lingodb::ast::JoinType>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_Type: // Type
      case symbol_kind::S_SimpleType: // SimpleType
      case symbol_kind::S_ConstTypename: // ConstTypename
      case symbol_kind::S_Numeric_with_opt_lenghth: // Numeric_with_opt_lenghth
      case symbol_kind::S_Numeric: // Numeric
      case symbol_kind::S_Character: // Character
      case symbol_kind::S_ConstCharacter: // ConstCharacter
      case symbol_kind::S_CharacterWithLength: // CharacterWithLength
      case symbol_kind::S_CharacterWithoutLength: // CharacterWithoutLength
      case symbol_kind::S_character: // character
      case symbol_kind::S_ConstDatetime: // ConstDatetime
      case symbol_kind::S_ConstInterval: // ConstInterval
      case symbol_kind::S_Typename: // Typename
         value.move<lingodb::ast::LogicalTypeWithMods>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_nulls_order: // opt_nulls_order
         value.move<lingodb::ast::OrderByNullType>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_asc_desc: // opt_asc_desc
         value.move<lingodb::ast::OrderType>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_sub_type: // sub_type
         value.move<lingodb::ast::SubqueryType>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_join_qual: // join_qual
         value.move<lingodb::ast::jointCondOrUsingCols>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_interval: // opt_interval
         value.move<std::optional<lingodb::ast::SQLAbstractLogicalType>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_sort_clause: // opt_sort_clause
         value.move<std::optional<std::shared_ptr<lingodb::ast::OrderByModifier>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_case_arg: // case_arg
         value.move<std::optional<std::shared_ptr<lingodb::ast::ParsedExpression>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_select_limit: // opt_select_limit
         value.move<std::optional<std::shared_ptr<lingodb::ast::ResultModifier>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_copy_opt_item: // copy_opt_item
         value.move<std::pair<std::string, std::string>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_alias_clause: // alias_clause
      case symbol_kind::S_opt_alias_clause: // opt_alias_clause
         value.move<std::pair<std::string, std::vector<std::string>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_extract_arg: // extract_arg
         value.move<std::shared_ptr<ast::ConstantExpression>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_CopyStmt: // CopyStmt
         value.move<std::shared_ptr<ast::CopyNode>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_VariableSetStmt: // VariableSetStmt
      case symbol_kind::S_set_rest: // set_rest
      case symbol_kind::S_generic_set: // generic_set
      case symbol_kind::S_set_rest_more: // set_rest_more
         value.move<std::shared_ptr<ast::SetVariableStatement>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_agg_expr: // agg_expr
         value.move<std::shared_ptr<lingodb::ast::AggregationNode>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_toplevel_stmt: // toplevel_stmt
      case symbol_kind::S_stmt: // stmt
         value.move<std::shared_ptr<lingodb::ast::AstNode>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_and_a_expr: // and_a_expr
      case symbol_kind::S_or_a_expr: // or_a_expr
         value.move<std::shared_ptr<lingodb::ast::ConjunctionExpression>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_ColConstraint: // ColConstraint
      case symbol_kind::S_ColConstraintElem: // ColConstraintElem
      case symbol_kind::S_ConstraintElem: // ConstraintElem
         value.move<std::shared_ptr<lingodb::ast::Constraint>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_CreateStmt: // CreateStmt
         value.move<std::shared_ptr<lingodb::ast::CreateNode>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_from_list: // from_list
         value.move<std::shared_ptr<lingodb::ast::CrossProductRef>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_values_clause: // values_clause
         value.move<std::shared_ptr<lingodb::ast::ExpressionListRef>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_func_application: // func_application
      case symbol_kind::S_func_expr: // func_expr
      case symbol_kind::S_func_expr_common_subexpr: // func_expr_common_subexpr
         value.move<std::shared_ptr<lingodb::ast::FunctionExpression>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_group_clause: // group_clause
      case symbol_kind::S_group_clause_with_alias: // group_clause_with_alias
         value.move<std::shared_ptr<lingodb::ast::GroupByNode>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_InsertStmt: // InsertStmt
      case symbol_kind::S_insert_rest: // insert_rest
         value.move<std::shared_ptr<lingodb::ast::InsertNode>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_select_limit: // select_limit
      case symbol_kind::S_limit_clause: // limit_clause
         value.move<std::shared_ptr<lingodb::ast::LimitModifier>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_sortby: // sortby
         value.move<std::shared_ptr<lingodb::ast::OrderByElement>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_sort_clause: // sort_clause
         value.move<std::shared_ptr<lingodb::ast::OrderByModifier>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_var_value: // var_value
      case symbol_kind::S_opt_boolean_or_string: // opt_boolean_or_string
      case symbol_kind::S_NonReservedWord_or_Sconst: // NonReservedWord_or_Sconst
      case symbol_kind::S_group_by_item: // group_by_item
      case symbol_kind::S_group_by_item_with_alias: // group_by_item_with_alias
      case symbol_kind::S_having_clause: // having_clause
      case symbol_kind::S_offset_clause: // offset_clause
      case symbol_kind::S_select_limit_value: // select_limit_value
      case symbol_kind::S_select_offset_value: // select_offset_value
      case symbol_kind::S_where_clause: // where_clause
      case symbol_kind::S_a_expr: // a_expr
      case symbol_kind::S_b_expr: // b_expr
      case symbol_kind::S_c_expr: // c_expr
      case symbol_kind::S_case_expr: // case_expr
      case symbol_kind::S_case_default: // case_default
      case symbol_kind::S_columnref: // columnref
      case symbol_kind::S_func_arg_expr: // func_arg_expr
      case symbol_kind::S_cast_expr: // cast_expr
      case symbol_kind::S_indirection: // indirection
      case symbol_kind::S_indirection_el: // indirection_el
      case symbol_kind::S_target_el: // target_el
      case symbol_kind::S_NumericOnly: // NumericOnly
      case symbol_kind::S_AexprConst: // AexprConst
      case symbol_kind::S_Iconst: // Iconst
      case symbol_kind::S_SignedIconst: // SignedIconst
      case symbol_kind::S_Fconst: // Fconst
      case symbol_kind::S_Sconst: // Sconst
      case symbol_kind::S_Bconst: // Bconst
         value.move<std::shared_ptr<lingodb::ast::ParsedExpression>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_pipe_start: // pipe_start
      case symbol_kind::S_pipe_operator: // pipe_operator
         value.move<std::shared_ptr<lingodb::ast::PipeOperator>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_select_with_parens: // select_with_parens
      case symbol_kind::S_select_no_parens: // select_no_parens
      case symbol_kind::S_select_clause: // select_clause
      case symbol_kind::S_simple_select: // simple_select
      case symbol_kind::S_with_clause: // with_clause
      case symbol_kind::S_cte_list: // cte_list
      case symbol_kind::S_common_table_expr: // common_table_expr
         value.move<std::shared_ptr<lingodb::ast::QueryNode>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_set_list: // set_list
         value.move<std::shared_ptr<lingodb::ast::SetColumnExpression>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_TableElement: // TableElement
      case symbol_kind::S_TableConstraint: // TableConstraint
      case symbol_kind::S_columnElement: // columnElement
         value.move<std::shared_ptr<lingodb::ast::TableElement>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_SelectStmt: // SelectStmt
      case symbol_kind::S_PipeSQLStmt: // PipeSQLStmt
      case symbol_kind::S_classic_select_and_pipe_sql_with_parens: // classic_select_and_pipe_sql_with_parens
      case symbol_kind::S_pipe_sql_with_parens: // pipe_sql_with_parens
      case symbol_kind::S_pipe_sql_no_parens: // pipe_sql_no_parens
      case symbol_kind::S_pipe_or_select_clause: // pipe_or_select_clause
      case symbol_kind::S_PreparableStmt: // PreparableStmt
         value.move<std::shared_ptr<lingodb::ast::TableProducer>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_from_clause: // opt_from_clause
      case symbol_kind::S_from_clause: // from_clause
      case symbol_kind::S_table_ref: // table_ref
      case symbol_kind::S_joined_table: // joined_table
         value.move<std::shared_ptr<lingodb::ast::TableRef>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_target_list: // opt_target_list
         value.move<std::shared_ptr<lingodb::ast::TargetList>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_type_modifier: // type_modifier
         value.move<std::shared_ptr<lingodb::ast::Value>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_window_func_expr: // window_func_expr
      case symbol_kind::S_over_clause: // over_clause
      case symbol_kind::S_window_specification: // window_specification
         value.move<std::shared_ptr<lingodb::ast::WindowExpression>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_frame_clause: // opt_frame_clause
      case symbol_kind::S_frame_extent: // frame_extent
      case symbol_kind::S_frame_bound: // frame_bound
         value.move<std::shared_ptr<lingodb::ast::WindowFrame>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_FCONST: // FCONST
      case symbol_kind::S_IDENT: // IDENT
      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_STRING_VALUE: // STRING_VALUE
      case symbol_kind::S_BIT_VALUE: // "bit_string"
      case symbol_kind::S_HEX_VALUE: // "hex_string"
      case symbol_kind::S_NATIONAL_VALUE: // "nat_string"
      case symbol_kind::S_PERCENT: // "%"
      case symbol_kind::S_PLUS: // "+"
      case symbol_kind::S_SLASH: // "/"
      case symbol_kind::S_STAR: // "*"
      case symbol_kind::S_MINUS: // "-"
      case symbol_kind::S_EQUAL: // "="
      case symbol_kind::S_NOT_EQUAL: // "<>"
      case symbol_kind::S_LESS_EQUAL: // "<="
      case symbol_kind::S_LESS: // "<"
      case symbol_kind::S_GREATER_EQUAL: // ">="
      case symbol_kind::S_GREATER: // ">"
      case symbol_kind::S_UIDENT: // UIDENT
      case symbol_kind::S_SCONST: // SCONST
      case symbol_kind::S_USCONST: // USCONST
      case symbol_kind::S_BCONST: // BCONST
      case symbol_kind::S_XCONST: // XCONST
      case symbol_kind::S_Op: // Op
      case symbol_kind::S_TYPECAST: // TYPECAST
      case symbol_kind::S_ABORT_P: // ABORT_P
      case symbol_kind::S_ABSENT: // ABSENT
      case symbol_kind::S_ABSOLUTE_P: // ABSOLUTE_P
      case symbol_kind::S_ACCESS: // ACCESS
      case symbol_kind::S_ACTION: // ACTION
      case symbol_kind::S_ADD_P: // ADD_P
      case symbol_kind::S_ADMIN: // ADMIN
      case symbol_kind::S_AFTER: // AFTER
      case symbol_kind::S_AGGREGATE: // AGGREGATE
      case symbol_kind::S_ALL: // ALL
      case symbol_kind::S_ALSO: // ALSO
      case symbol_kind::S_ALTER: // ALTER
      case symbol_kind::S_ALWAYS: // ALWAYS
      case symbol_kind::S_ANALYSE: // ANALYSE
      case symbol_kind::S_ANALYZE: // ANALYZE
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ANY: // ANY
      case symbol_kind::S_ARRAY: // ARRAY
      case symbol_kind::S_AS: // AS
      case symbol_kind::S_ASC: // ASC
      case symbol_kind::S_ASENSITIVE: // ASENSITIVE
      case symbol_kind::S_ASSERTION: // ASSERTION
      case symbol_kind::S_ASSIGNMENT: // ASSIGNMENT
      case symbol_kind::S_ASYMMETRIC: // ASYMMETRIC
      case symbol_kind::S_ATOMIC: // ATOMIC
      case symbol_kind::S_AT: // AT
      case symbol_kind::S_ATTACH: // ATTACH
      case symbol_kind::S_ATTRIBUTE: // ATTRIBUTE
      case symbol_kind::S_AUTHORIZATION: // AUTHORIZATION
      case symbol_kind::S_BACKWARD: // BACKWARD
      case symbol_kind::S_BEFORE: // BEFORE
      case symbol_kind::S_BEGIN_P: // BEGIN_P
      case symbol_kind::S_BETWEEN: // BETWEEN
      case symbol_kind::S_BIGINT: // BIGINT
      case symbol_kind::S_BINARY: // BINARY
      case symbol_kind::S_BIT: // BIT
      case symbol_kind::S_BOOLEAN_P: // BOOLEAN_P
      case symbol_kind::S_BOTH: // BOTH
      case symbol_kind::S_BREADTH: // BREADTH
      case symbol_kind::S_BY: // BY
      case symbol_kind::S_CACHE: // CACHE
      case symbol_kind::S_CALL: // CALL
      case symbol_kind::S_CALLED: // CALLED
      case symbol_kind::S_CASCADE: // CASCADE
      case symbol_kind::S_CASCADED: // CASCADED
      case symbol_kind::S_CASE: // CASE
      case symbol_kind::S_CAST: // CAST
      case symbol_kind::S_CATALOG_P: // CATALOG_P
      case symbol_kind::S_CHAIN: // CHAIN
      case symbol_kind::S_CHAR_P: // CHAR_P
      case symbol_kind::S_CHARACTER: // CHARACTER
      case symbol_kind::S_CHARACTERISTICS: // CHARACTERISTICS
      case symbol_kind::S_CHECK: // CHECK
      case symbol_kind::S_CHECKPOINT: // CHECKPOINT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_CLOSE: // CLOSE
      case symbol_kind::S_CLUSTER: // CLUSTER
      case symbol_kind::S_COALESCE: // COALESCE
      case symbol_kind::S_COLLATE: // COLLATE
      case symbol_kind::S_COLLATION: // COLLATION
      case symbol_kind::S_COLUMN: // COLUMN
      case symbol_kind::S_COLUMNS: // COLUMNS
      case symbol_kind::S_COMMENT: // COMMENT
      case symbol_kind::S_COMMENTS: // COMMENTS
      case symbol_kind::S_COMMIT: // COMMIT
      case symbol_kind::S_COMMITTED: // COMMITTED
      case symbol_kind::S_COMPRESSION: // COMPRESSION
      case symbol_kind::S_CONCURRENTLY: // CONCURRENTLY
      case symbol_kind::S_CONDITIONAL: // CONDITIONAL
      case symbol_kind::S_CONFIGURATION: // CONFIGURATION
      case symbol_kind::S_CONFLICT: // CONFLICT
      case symbol_kind::S_CONNECTION: // CONNECTION
      case symbol_kind::S_CONSTRAINT: // CONSTRAINT
      case symbol_kind::S_CONSTRAINTS: // CONSTRAINTS
      case symbol_kind::S_CONTENT_P: // CONTENT_P
      case symbol_kind::S_CONTINUE_P: // CONTINUE_P
      case symbol_kind::S_CONVERSION_P: // CONVERSION_P
      case symbol_kind::S_COPY: // COPY
      case symbol_kind::S_COST: // COST
      case symbol_kind::S_CREATE: // CREATE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_CSV: // CSV
      case symbol_kind::S_CUBE: // CUBE
      case symbol_kind::S_CURRENT_P: // CURRENT_P
      case symbol_kind::S_CURRENT_CATALOG: // CURRENT_CATALOG
      case symbol_kind::S_CURRENT_DATE: // CURRENT_DATE
      case symbol_kind::S_CURRENT_ROLE: // CURRENT_ROLE
      case symbol_kind::S_CURRENT_SCHEMA: // CURRENT_SCHEMA
      case symbol_kind::S_CURRENT_TIME: // CURRENT_TIME
      case symbol_kind::S_CURRENT_TIMESTAMP: // CURRENT_TIMESTAMP
      case symbol_kind::S_CURRENT_USER: // CURRENT_USER
      case symbol_kind::S_CURSOR: // CURSOR
      case symbol_kind::S_CYCLE: // CYCLE
      case symbol_kind::S_DATA_P: // DATA_P
      case symbol_kind::S_DATABASE: // DATABASE
      case symbol_kind::S_DAY_P: // DAY_P
      case symbol_kind::S_DEALLOCATE: // DEALLOCATE
      case symbol_kind::S_DEC: // DEC
      case symbol_kind::S_DECIMAL_P: // DECIMAL_P
      case symbol_kind::S_DECLARE: // DECLARE
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_DEFAULTS: // DEFAULTS
      case symbol_kind::S_DATE_P: // DATE_P
      case symbol_kind::S_DEFERRABLE: // DEFERRABLE
      case symbol_kind::S_DEFERRED: // DEFERRED
      case symbol_kind::S_DEFINER: // DEFINER
      case symbol_kind::S_DELETE_P: // DELETE_P
      case symbol_kind::S_DELIMITER: // DELIMITER
      case symbol_kind::S_DELIMITERS: // DELIMITERS
      case symbol_kind::S_DEPENDS: // DEPENDS
      case symbol_kind::S_DEPTH: // DEPTH
      case symbol_kind::S_DESC: // DESC
      case symbol_kind::S_DETACH: // DETACH
      case symbol_kind::S_DICTIONARY: // DICTIONARY
      case symbol_kind::S_DISABLE_P: // DISABLE_P
      case symbol_kind::S_DISCARD: // DISCARD
      case symbol_kind::S_DISTINCT: // DISTINCT
      case symbol_kind::S_DO: // DO
      case symbol_kind::S_DOCUMENT_P: // DOCUMENT_P
      case symbol_kind::S_DOMAIN_P: // DOMAIN_P
      case symbol_kind::S_DOUBLE_P: // DOUBLE_P
      case symbol_kind::S_DROP: // DROP
      case symbol_kind::S_EACH: // EACH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EMPTY_P: // EMPTY_P
      case symbol_kind::S_ENABLE_P: // ENABLE_P
      case symbol_kind::S_ENCODING: // ENCODING
      case symbol_kind::S_ENCRYPTED: // ENCRYPTED
      case symbol_kind::S_END_P: // END_P
      case symbol_kind::S_ENFORCED: // ENFORCED
      case symbol_kind::S_ENUM_P: // ENUM_P
      case symbol_kind::S_ERROR_P: // ERROR_P
      case symbol_kind::S_ESCAPE: // ESCAPE
      case symbol_kind::S_EVENT: // EVENT
      case symbol_kind::S_EXCEPT: // EXCEPT
      case symbol_kind::S_EXCLUDE: // EXCLUDE
      case symbol_kind::S_EXCLUDING: // EXCLUDING
      case symbol_kind::S_EXCLUSIVE: // EXCLUSIVE
      case symbol_kind::S_EXECUTE: // EXECUTE
      case symbol_kind::S_EXISTS: // EXISTS
      case symbol_kind::S_EXPLAIN: // EXPLAIN
      case symbol_kind::S_EXPRESSION: // EXPRESSION
      case symbol_kind::S_EXTENSION: // EXTENSION
      case symbol_kind::S_EXTERNAL: // EXTERNAL
      case symbol_kind::S_EXTRACT: // EXTRACT
      case symbol_kind::S_FALSE_P: // FALSE_P
      case symbol_kind::S_FAMILY: // FAMILY
      case symbol_kind::S_FETCH: // FETCH
      case symbol_kind::S_FILTER: // FILTER
      case symbol_kind::S_FINALIZE: // FINALIZE
      case symbol_kind::S_FIRST_P: // FIRST_P
      case symbol_kind::S_FLOAT_P: // FLOAT_P
      case symbol_kind::S_FOLLOWING: // FOLLOWING
      case symbol_kind::S_FOR: // FOR
      case symbol_kind::S_FORCE: // FORCE
      case symbol_kind::S_FOREIGN: // FOREIGN
      case symbol_kind::S_FORMAT: // FORMAT
      case symbol_kind::S_FORWARD: // FORWARD
      case symbol_kind::S_FREEZE: // FREEZE
      case symbol_kind::S_FROM: // FROM
      case symbol_kind::S_FULL: // FULL
      case symbol_kind::S_FUNCTION: // FUNCTION
      case symbol_kind::S_FUNCTIONS: // FUNCTIONS
      case symbol_kind::S_GENERATED: // GENERATED
      case symbol_kind::S_GLOBAL: // GLOBAL
      case symbol_kind::S_GRANT: // GRANT
      case symbol_kind::S_GRANTED: // GRANTED
      case symbol_kind::S_GREATEST: // GREATEST
      case symbol_kind::S_GROUP_P: // GROUP_P
      case symbol_kind::S_GROUPING: // GROUPING
      case symbol_kind::S_GROUPS: // GROUPS
      case symbol_kind::S_HANDLER: // HANDLER
      case symbol_kind::S_HAVING: // HAVING
      case symbol_kind::S_HEADER_P: // HEADER_P
      case symbol_kind::S_HOLD: // HOLD
      case symbol_kind::S_HOUR_P: // HOUR_P
      case symbol_kind::S_IDENTITY_P: // IDENTITY_P
      case symbol_kind::S_IF_P: // IF_P
      case symbol_kind::S_ILIKE: // ILIKE
      case symbol_kind::S_IMMEDIATE: // IMMEDIATE
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_IMPLICIT_P: // IMPLICIT_P
      case symbol_kind::S_IMPORT_P: // IMPORT_P
      case symbol_kind::S_IN_P: // IN_P
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_INCLUDING: // INCLUDING
      case symbol_kind::S_INCREMENT: // INCREMENT
      case symbol_kind::S_INDENT: // INDENT
      case symbol_kind::S_INDEX: // INDEX
      case symbol_kind::S_INDEXES: // INDEXES
      case symbol_kind::S_INHERIT: // INHERIT
      case symbol_kind::S_INHERITS: // INHERITS
      case symbol_kind::S_INITIALLY: // INITIALLY
      case symbol_kind::S_INLINE_P: // INLINE_P
      case symbol_kind::S_INNER_P: // INNER_P
      case symbol_kind::S_INOUT: // INOUT
      case symbol_kind::S_INPUT_P: // INPUT_P
      case symbol_kind::S_INSENSITIVE: // INSENSITIVE
      case symbol_kind::S_INSERT: // INSERT
      case symbol_kind::S_INSTEAD: // INSTEAD
      case symbol_kind::S_INT_P: // INT_P
      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_INTERSECT: // INTERSECT
      case symbol_kind::S_INTERVAL: // INTERVAL
      case symbol_kind::S_INTO: // INTO
      case symbol_kind::S_INVOKER: // INVOKER
      case symbol_kind::S_IS: // IS
      case symbol_kind::S_ISNULL: // ISNULL
      case symbol_kind::S_ISOLATION: // ISOLATION
      case symbol_kind::S_JOIN: // JOIN
      case symbol_kind::S_JSON: // JSON
      case symbol_kind::S_JSON_ARRAY: // JSON_ARRAY
      case symbol_kind::S_JSON_ARRAYAGG: // JSON_ARRAYAGG
      case symbol_kind::S_JSON_EXISTS: // JSON_EXISTS
      case symbol_kind::S_JSON_OBJECT: // JSON_OBJECT
      case symbol_kind::S_JSON_OBJECTAGG: // JSON_OBJECTAGG
      case symbol_kind::S_JSON_QUERY: // JSON_QUERY
      case symbol_kind::S_JSON_SCALAR: // JSON_SCALAR
      case symbol_kind::S_JSON_SERIALIZE: // JSON_SERIALIZE
      case symbol_kind::S_JSON_TABLE: // JSON_TABLE
      case symbol_kind::S_JSON_VALUE: // JSON_VALUE
      case symbol_kind::S_KEEP: // KEEP
      case symbol_kind::S_KEY: // KEY
      case symbol_kind::S_KEYS: // KEYS
      case symbol_kind::S_LABEL: // LABEL
      case symbol_kind::S_LANGUAGE: // LANGUAGE
      case symbol_kind::S_LARGE_P: // LARGE_P
      case symbol_kind::S_LAST_P: // LAST_P
      case symbol_kind::S_LATERAL_P: // LATERAL_P
      case symbol_kind::S_LEADING: // LEADING
      case symbol_kind::S_LEAKPROOF: // LEAKPROOF
      case symbol_kind::S_LEAST: // LEAST
      case symbol_kind::S_LEFT: // LEFT
      case symbol_kind::S_LEVEL: // LEVEL
      case symbol_kind::S_LIKE: // LIKE
      case symbol_kind::S_LIMIT: // LIMIT
      case symbol_kind::S_LISTEN: // LISTEN
      case symbol_kind::S_LOAD: // LOAD
      case symbol_kind::S_LOCAL: // LOCAL
      case symbol_kind::S_LOCALTIME: // LOCALTIME
      case symbol_kind::S_LOCALTIMESTAMP: // LOCALTIMESTAMP
      case symbol_kind::S_LOCATION: // LOCATION
      case symbol_kind::S_LOCK_P: // LOCK_P
      case symbol_kind::S_LOCKED: // LOCKED
      case symbol_kind::S_LOGGED: // LOGGED
      case symbol_kind::S_MAPPING: // MAPPING
      case symbol_kind::S_MATCH: // MATCH
      case symbol_kind::S_MATCHED: // MATCHED
      case symbol_kind::S_MATERIALIZED: // MATERIALIZED
      case symbol_kind::S_MAXVALUE: // MAXVALUE
      case symbol_kind::S_MERGE: // MERGE
      case symbol_kind::S_MERGE_ACTION: // MERGE_ACTION
      case symbol_kind::S_METHOD: // METHOD
      case symbol_kind::S_MINUTE_P: // MINUTE_P
      case symbol_kind::S_MINVALUE: // MINVALUE
      case symbol_kind::S_MODE: // MODE
      case symbol_kind::S_MONTH_P: // MONTH_P
      case symbol_kind::S_MOVE: // MOVE
      case symbol_kind::S_NAME_P: // NAME_P
      case symbol_kind::S_NAMES: // NAMES
      case symbol_kind::S_NATIONAL: // NATIONAL
      case symbol_kind::S_NATURAL: // NATURAL
      case symbol_kind::S_NCHAR: // NCHAR
      case symbol_kind::S_NESTED: // NESTED
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_NEXT: // NEXT
      case symbol_kind::S_NFC: // NFC
      case symbol_kind::S_NFD: // NFD
      case symbol_kind::S_NFKC: // NFKC
      case symbol_kind::S_NFKD: // NFKD
      case symbol_kind::S_NO: // NO
      case symbol_kind::S_NONE: // NONE
      case symbol_kind::S_NORMALIZE: // NORMALIZE
      case symbol_kind::S_NORMALIZED: // NORMALIZED
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTHING: // NOTHING
      case symbol_kind::S_NOTIFY: // NOTIFY
      case symbol_kind::S_NOTNULL: // NOTNULL
      case symbol_kind::S_NOWAIT: // NOWAIT
      case symbol_kind::S_NULL_P: // NULL_P
      case symbol_kind::S_NULLIF: // NULLIF
      case symbol_kind::S_NULLS_P: // NULLS_P
      case symbol_kind::S_NUMERIC: // NUMERIC
      case symbol_kind::S_OBJECT_P: // OBJECT_P
      case symbol_kind::S_OBJECTS_P: // OBJECTS_P
      case symbol_kind::S_OF: // OF
      case symbol_kind::S_OFF: // OFF
      case symbol_kind::S_OFFSET: // OFFSET
      case symbol_kind::S_OIDS: // OIDS
      case symbol_kind::S_OLD: // OLD
      case symbol_kind::S_OMIT: // OMIT
      case symbol_kind::S_ON: // ON
      case symbol_kind::S_ONLY: // ONLY
      case symbol_kind::S_OPERATOR: // OPERATOR
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_OPTIONS: // OPTIONS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_ORDER: // ORDER
      case symbol_kind::S_ORDINALITY: // ORDINALITY
      case symbol_kind::S_OTHERS: // OTHERS
      case symbol_kind::S_OUT_P: // OUT_P
      case symbol_kind::S_OUTER_P: // OUTER_P
      case symbol_kind::S_OVER: // OVER
      case symbol_kind::S_OVERLAPS: // OVERLAPS
      case symbol_kind::S_OVERLAY: // OVERLAY
      case symbol_kind::S_OVERRIDING: // OVERRIDING
      case symbol_kind::S_OWNED: // OWNED
      case symbol_kind::S_OWNER: // OWNER
      case symbol_kind::S_PARALLEL: // PARALLEL
      case symbol_kind::S_PARAMETER: // PARAMETER
      case symbol_kind::S_PARSER: // PARSER
      case symbol_kind::S_PARTIAL: // PARTIAL
      case symbol_kind::S_PARTITION: // PARTITION
      case symbol_kind::S_PASSING: // PASSING
      case symbol_kind::S_PASSWORD: // PASSWORD
      case symbol_kind::S_PATH: // PATH
      case symbol_kind::S_PERIOD: // PERIOD
      case symbol_kind::S_PLACING: // PLACING
      case symbol_kind::S_PLAN: // PLAN
      case symbol_kind::S_PLANS: // PLANS
      case symbol_kind::S_POLICY: // POLICY
      case symbol_kind::S_POSITION: // POSITION
      case symbol_kind::S_PRECEDING: // PRECEDING
      case symbol_kind::S_PRECISION: // PRECISION
      case symbol_kind::S_PRESERVE: // PRESERVE
      case symbol_kind::S_PREPARE: // PREPARE
      case symbol_kind::S_PREPARED: // PREPARED
      case symbol_kind::S_PRIMARY: // PRIMARY
      case symbol_kind::S_PRIOR: // PRIOR
      case symbol_kind::S_PRIVILEGES: // PRIVILEGES
      case symbol_kind::S_PROCEDURAL: // PROCEDURAL
      case symbol_kind::S_PROCEDURE: // PROCEDURE
      case symbol_kind::S_PROCEDURES: // PROCEDURES
      case symbol_kind::S_PROGRAM: // PROGRAM
      case symbol_kind::S_PUBLICATION: // PUBLICATION
      case symbol_kind::S_QUOTES: // QUOTES
      case symbol_kind::S_EXTEND: // EXTEND
      case symbol_kind::S_RANGE: // RANGE
      case symbol_kind::S_READ: // READ
      case symbol_kind::S_REAL: // REAL
      case symbol_kind::S_REASSIGN: // REASSIGN
      case symbol_kind::S_RECURSIVE: // RECURSIVE
      case symbol_kind::S_REF_P: // REF_P
      case symbol_kind::S_REFERENCES: // REFERENCES
      case symbol_kind::S_REFERENCING: // REFERENCING
      case symbol_kind::S_REFRESH: // REFRESH
      case symbol_kind::S_REINDEX: // REINDEX
      case symbol_kind::S_RELATIVE_P: // RELATIVE_P
      case symbol_kind::S_RELEASE: // RELEASE
      case symbol_kind::S_RENAME: // RENAME
      case symbol_kind::S_REPEATABLE: // REPEATABLE
      case symbol_kind::S_REPLACE: // REPLACE
      case symbol_kind::S_REPLICA: // REPLICA
      case symbol_kind::S_RESET: // RESET
      case symbol_kind::S_RESTART: // RESTART
      case symbol_kind::S_RESTRICT: // RESTRICT
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RETURNING: // RETURNING
      case symbol_kind::S_RETURNS: // RETURNS
      case symbol_kind::S_REVOKE: // REVOKE
      case symbol_kind::S_RIGHT: // RIGHT
      case symbol_kind::S_ROLE: // ROLE
      case symbol_kind::S_ROLLBACK: // ROLLBACK
      case symbol_kind::S_ROLLUP: // ROLLUP
      case symbol_kind::S_ROUTINE: // ROUTINE
      case symbol_kind::S_ROUTINES: // ROUTINES
      case symbol_kind::S_ROW: // ROW
      case symbol_kind::S_ROWS: // ROWS
      case symbol_kind::S_RULE: // RULE
      case symbol_kind::S_SAVEPOINT: // SAVEPOINT
      case symbol_kind::S_SCALAR: // SCALAR
      case symbol_kind::S_SCHEMA: // SCHEMA
      case symbol_kind::S_SCHEMAS: // SCHEMAS
      case symbol_kind::S_SCROLL: // SCROLL
      case symbol_kind::S_SEARCH: // SEARCH
      case symbol_kind::S_SECOND_P: // SECOND_P
      case symbol_kind::S_SECURITY: // SECURITY
      case symbol_kind::S_SELECT: // SELECT
      case symbol_kind::S_SEQUENCE: // SEQUENCE
      case symbol_kind::S_SEQUENCES: // SEQUENCES
      case symbol_kind::S_SERIALIZABLE: // SERIALIZABLE
      case symbol_kind::S_SERVER: // SERVER
      case symbol_kind::S_SESSION: // SESSION
      case symbol_kind::S_SESSION_USER: // SESSION_USER
      case symbol_kind::S_SET: // SET
      case symbol_kind::S_SETS: // SETS
      case symbol_kind::S_SETOF: // SETOF
      case symbol_kind::S_SHARE: // SHARE
      case symbol_kind::S_SHOW: // SHOW
      case symbol_kind::S_SIMILAR: // SIMILAR
      case symbol_kind::S_SIMPLE: // SIMPLE
      case symbol_kind::S_SKIP: // SKIP
      case symbol_kind::S_SMALLINT: // SMALLINT
      case symbol_kind::S_SNAPSHOT: // SNAPSHOT
      case symbol_kind::S_SOME: // SOME
      case symbol_kind::S_SOURCE: // SOURCE
      case symbol_kind::S_SQL_P: // SQL_P
      case symbol_kind::S_STABLE: // STABLE
      case symbol_kind::S_STANDALONE_P: // STANDALONE_P
      case symbol_kind::S_START: // START
      case symbol_kind::S_STATEMENT: // STATEMENT
      case symbol_kind::S_STATISTICS: // STATISTICS
      case symbol_kind::S_STDIN: // STDIN
      case symbol_kind::S_STDOUT: // STDOUT
      case symbol_kind::S_STORAGE: // STORAGE
      case symbol_kind::S_STORED: // STORED
      case symbol_kind::S_STRICT_P: // STRICT_P
      case symbol_kind::S_STRING_P: // STRING_P
      case symbol_kind::S_STRIP_P: // STRIP_P
      case symbol_kind::S_SUBSCRIPTION: // SUBSCRIPTION
      case symbol_kind::S_SUBSTRING: // SUBSTRING
      case symbol_kind::S_SUPPORT: // SUPPORT
      case symbol_kind::S_SYMMETRIC: // SYMMETRIC
      case symbol_kind::S_SYSID: // SYSID
      case symbol_kind::S_SYSTEM_P: // SYSTEM_P
      case symbol_kind::S_SYSTEM_USER: // SYSTEM_USER
      case symbol_kind::S_TABLE: // TABLE
      case symbol_kind::S_TABLES: // TABLES
      case symbol_kind::S_TABLESAMPLE: // TABLESAMPLE
      case symbol_kind::S_TABLESPACE: // TABLESPACE
      case symbol_kind::S_TARGET: // TARGET
      case symbol_kind::S_TEMP: // TEMP
      case symbol_kind::S_TEMPLATE: // TEMPLATE
      case symbol_kind::S_TEMPORARY: // TEMPORARY
      case symbol_kind::S_TEXT_P: // TEXT_P
      case symbol_kind::S_THEN: // THEN
      case symbol_kind::S_TIES: // TIES
      case symbol_kind::S_TIME: // TIME
      case symbol_kind::S_TIMESTAMP: // TIMESTAMP
      case symbol_kind::S_TO: // TO
      case symbol_kind::S_TRAILING: // TRAILING
      case symbol_kind::S_TRANSACTION: // TRANSACTION
      case symbol_kind::S_TRANSFORM: // TRANSFORM
      case symbol_kind::S_TREAT: // TREAT
      case symbol_kind::S_TRIGGER: // TRIGGER
      case symbol_kind::S_TRIM: // TRIM
      case symbol_kind::S_TRUE_P: // TRUE_P
      case symbol_kind::S_TRUNCATE: // TRUNCATE
      case symbol_kind::S_TRUSTED: // TRUSTED
      case symbol_kind::S_TYPE_P: // TYPE_P
      case symbol_kind::S_TYPES_P: // TYPES_P
      case symbol_kind::S_UESCAPE: // UESCAPE
      case symbol_kind::S_UNBOUNDED: // UNBOUNDED
      case symbol_kind::S_UNCONDITIONAL: // UNCONDITIONAL
      case symbol_kind::S_UNCOMMITTED: // UNCOMMITTED
      case symbol_kind::S_UNENCRYPTED: // UNENCRYPTED
      case symbol_kind::S_UNION: // UNION
      case symbol_kind::S_UNIQUE: // UNIQUE
      case symbol_kind::S_UNKNOWN: // UNKNOWN
      case symbol_kind::S_UNLISTEN: // UNLISTEN
      case symbol_kind::S_UNLOGGED: // UNLOGGED
      case symbol_kind::S_UNTIL: // UNTIL
      case symbol_kind::S_UPDATE: // UPDATE
      case symbol_kind::S_USER: // USER
      case symbol_kind::S_USING: // USING
      case symbol_kind::S_VACUUM: // VACUUM
      case symbol_kind::S_VALID: // VALID
      case symbol_kind::S_VALIDATE: // VALIDATE
      case symbol_kind::S_VALIDATOR: // VALIDATOR
      case symbol_kind::S_VALUE_P: // VALUE_P
      case symbol_kind::S_VALUES: // VALUES
      case symbol_kind::S_VARCHAR: // VARCHAR
      case symbol_kind::S_VARIADIC: // VARIADIC
      case symbol_kind::S_VARYING: // VARYING
      case symbol_kind::S_VERBOSE: // VERBOSE
      case symbol_kind::S_VERSION_P: // VERSION_P
      case symbol_kind::S_VIEW: // VIEW
      case symbol_kind::S_VIEWS: // VIEWS
      case symbol_kind::S_VIRTUAL: // VIRTUAL
      case symbol_kind::S_VOLATILE: // VOLATILE
      case symbol_kind::S_WHEN: // WHEN
      case symbol_kind::S_WHERE: // WHERE
      case symbol_kind::S_WHITESPACE_P: // WHITESPACE_P
      case symbol_kind::S_WINDOW: // WINDOW
      case symbol_kind::S_WITH: // WITH
      case symbol_kind::S_WITHIN: // WITHIN
      case symbol_kind::S_WITHOUT: // WITHOUT
      case symbol_kind::S_WORK: // WORK
      case symbol_kind::S_WRAPPER: // WRAPPER
      case symbol_kind::S_WRITE: // WRITE
      case symbol_kind::S_XML_P: // XML_P
      case symbol_kind::S_XMLATTRIBUTES: // XMLATTRIBUTES
      case symbol_kind::S_XMLCONCAT: // XMLCONCAT
      case symbol_kind::S_XMLELEMENT: // XMLELEMENT
      case symbol_kind::S_XMLEXISTS: // XMLEXISTS
      case symbol_kind::S_XMLFOREST: // XMLFOREST
      case symbol_kind::S_XMLNAMESPACES: // XMLNAMESPACES
      case symbol_kind::S_XMLPARSE: // XMLPARSE
      case symbol_kind::S_XMLPI: // XMLPI
      case symbol_kind::S_XMLROOT: // XMLROOT
      case symbol_kind::S_XMLSERIALIZE: // XMLSERIALIZE
      case symbol_kind::S_XMLTABLE: // XMLTABLE
      case symbol_kind::S_YEAR_P: // YEAR_P
      case symbol_kind::S_YES_P: // YES_P
      case symbol_kind::S_ZONE: // ZONE
      case symbol_kind::S_copy_file_name: // copy_file_name
      case symbol_kind::S_copy_delimiter: // copy_delimiter
      case symbol_kind::S_var_name: // var_name
      case symbol_kind::S_NonReservedWord: // NonReservedWord
      case symbol_kind::S_relation_expr: // relation_expr
      case symbol_kind::S_any_operator: // any_operator
      case symbol_kind::S_qual_Op: // qual_Op
      case symbol_kind::S_all_Op: // all_Op
      case symbol_kind::S_ColId: // ColId
      case symbol_kind::S_type_function_name: // type_function_name
      case symbol_kind::S_type_func_name_keyword: // type_func_name_keyword
      case symbol_kind::S_ColLabel: // ColLabel
      case symbol_kind::S_BareColLabel: // BareColLabel
      case symbol_kind::S_reserved_keyword: // reserved_keyword
      case symbol_kind::S_unreserved_keyword: // unreserved_keyword
      case symbol_kind::S_col_name_keyword: // col_name_keyword
      case symbol_kind::S_qualified_name: // qualified_name
      case symbol_kind::S_name: // name
      case symbol_kind::S_attr_name: // attr_name
      case symbol_kind::S_func_name: // func_name
      case symbol_kind::S_insert_target: // insert_target
      case symbol_kind::S_insert_column_item: // insert_column_item
         value.move<std::string>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_in_expr: // in_expr
         value.move<std::variant<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>, std::shared_ptr<lingodb::ast::SubqueryExpression>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_when_clause_list: // when_clause_list
         value.move<std::vector<lingodb::ast::CaseExpression::CaseCheck>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_copy_options: // copy_options
      case symbol_kind::S_copy_opt_list: // copy_opt_list
         value.move<std::vector<std::pair<std::string, std::string>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_stmtmulti: // stmtmulti
         value.move<std::vector<std::shared_ptr<lingodb::ast::AstNode>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_ColQualList: // ColQualList
         value.move<std::vector<std::shared_ptr<lingodb::ast::Constraint>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_func_expr_list: // func_expr_list
         value.move<std::vector<std::shared_ptr<lingodb::ast::FunctionExpression>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_sortby_list: // sortby_list
         value.move<std::vector<std::shared_ptr<lingodb::ast::OrderByElement>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_var_list: // var_list
      case symbol_kind::S_group_by_list: // group_by_list
      case symbol_kind::S_group_by_list_with_alias: // group_by_list_with_alias
      case symbol_kind::S_rollup_clause: // rollup_clause
      case symbol_kind::S_columnref_list: // columnref_list
      case symbol_kind::S_func_arg_list_opt: // func_arg_list_opt
      case symbol_kind::S_func_arg_list: // func_arg_list
      case symbol_kind::S_expr_list: // expr_list
      case symbol_kind::S_expr_list_with_alias: // expr_list_with_alias
      case symbol_kind::S_extract_list: // extract_list
      case symbol_kind::S_substr_list: // substr_list
      case symbol_kind::S_target_list: // target_list
      case symbol_kind::S_opt_partition_clause: // opt_partition_clause
         value.move<std::vector<std::shared_ptr<lingodb::ast::ParsedExpression>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_OptTableElementList: // OptTableElementList
      case symbol_kind::S_TableElementList: // TableElementList
         value.move<std::vector<std::shared_ptr<lingodb::ast::TableElement>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_type_modifiers: // opt_type_modifiers
      case symbol_kind::S_type_modifiers: // type_modifiers
         value.move<std::vector<std::shared_ptr<lingodb::ast::Value>>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_opt_name_list: // opt_name_list
      case symbol_kind::S_name_list: // name_list
      case symbol_kind::S_insert_column_list: // insert_column_list
         value.move<std::vector<std::string>>(YY_MOVE(s.value));
         break;

      case symbol_kind::S_INTEGER_VALUE: // "integer_value"
         value.move<uint64_t>(YY_MOVE(s.value));
         break;

      default:
         break;
   }

   location = YY_MOVE(s.location);
}

// by_kind.
inline parser::by_kind::by_kind() YY_NOEXCEPT
   : kind_(symbol_kind::S_YYEMPTY) {}

#if 201103L <= YY_CPLUSPLUS
inline parser::by_kind::by_kind(by_kind&& that) YY_NOEXCEPT
   : kind_(that.kind_) {
   that.clear();
}
#endif

inline parser::by_kind::by_kind(const by_kind& that) YY_NOEXCEPT
   : kind_(that.kind_) {}

inline parser::by_kind::by_kind(token_kind_type t) YY_NOEXCEPT
   : kind_(yytranslate_(t)) {}

inline void
parser::by_kind::clear() YY_NOEXCEPT {
   kind_ = symbol_kind::S_YYEMPTY;
}

inline void
parser::by_kind::move(by_kind& that) {
   kind_ = that.kind_;
   that.clear();
}

inline parser::symbol_kind_type
parser::by_kind::kind() const YY_NOEXCEPT {
   return kind_;
}

inline parser::symbol_kind_type
parser::by_kind::type_get() const YY_NOEXCEPT {
   return this->kind();
}

#line 5 "/home/bachmaier/projects/lingo-db/src/compiler/frontend/parser.y"
} //  lingodb
#line 15778 "/home/bachmaier/projects/lingo-db/include/lingodb/compiler/frontend/generated/parser.hpp"

#endif // !YY_YY_HOME_BACHMAIER_PROJECTS_LINGO_DB_INCLUDE_LINGODB_COMPILER_FRONTEND_GENERATED_PARSER_HPP_INCLUDED
