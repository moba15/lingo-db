#pragma once
#include <cstdint>
#include <string>
#define toAsciiASTPrefix std::string ast{}; \
   for (uint32_t i = 0; i < depth-1; ++i) { \
      ast.append("\t"); \
      ast.append(""); \
   } \
ast.append("\t"); \
ast.append("└──");
namespace lingodb::ast {
enum class QueryNodeType : uint8_t {
  SELECT_NODE,
  SET_OPERATION_NODE = 2,
  BOUND_SUBQUERY_NODE = 3,
  RECURSIVE_CTE_NODE = 4,
  CTE_NODE = 5
};
class QueryNode {
public:
   virtual ~QueryNode() ;
  explicit  QueryNode(QueryNodeType type) : type(type) {};

  //! The type of the query node, either SetOperation or Select
  QueryNodeType type;

   virtual std::string toString(uint32_t depth) = 0;

   virtual std::string toAsciiAST(uint32_t depth ) = 0;
};
}

