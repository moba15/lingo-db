#pragma once
#include "common/node_id_generator.h"
#include "lingodb/compiler/frontend/sql-parser/gen/location.hh"
namespace lingodb::ast {
enum class NodeType : uint8_t {
   QUERY_NODE = 1,
   EXPRESSION = 2,
   TABLE_REF = 3,
   PIPE_OP = 4,
   GROUP_BY = 5,
   AGGREGATION = 6,
   RESULT_MODIFIER = 7,
   EXTEND_NODE=8,

   //Bound
   BOUND_TABLE_REF = 20,
   BOUND_EXPRESSION = 22,
   BOUND_GROUP_BY = 25,
   BOUND_AGGREGATION = 26,
   BOUND_RESULT_MODIFIER = 27,
   BOUND_EXTEND_NODE = 28,
};
class AstNode {
   public:
   explicit AstNode(NodeType type) : nodeType(type) {}
   virtual ~AstNode() = default;

   NodeType nodeType;
   location loc;

   virtual std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) = 0;
};
} // namespace lingodb::ast
