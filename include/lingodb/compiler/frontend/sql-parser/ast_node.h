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
};
class AstNode {
   public:
   explicit AstNode(NodeType type) : nodeType(type) {}
   virtual ~AstNode() = default;

   NodeType nodeType;
   location loc;

   virtual std::string toAsciiAST(uint32_t depth) = 0;
   virtual std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) = 0;
};
} // namespace lingodb::ast
