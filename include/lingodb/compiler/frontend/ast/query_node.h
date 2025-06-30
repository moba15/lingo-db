#pragma once
#include "ast_node.h"
#include "result_modifier.h"
#include "table_producer.h"

#include <cstdint>
#include <memory>
#include <string>
#define toAsciiASTPrefix                      \
   std::string ast{};                         \
   for (uint32_t i = 0; i < depth - 1; ++i) { \
      ast.append("\t");                       \
                                              \
      ast.append("");                         \
   }                                          \
   ast.append("\t");                          \
   ast.append("└──");
namespace lingodb::ast {
enum class QueryNodeType : uint8_t {
   SELECT_NODE,
   SET_OPERATION_NODE = 2,
   BOUND_SUBQUERY_NODE = 3,
   RECURSIVE_CTE_NODE = 4,
   CTE_NODE = 5,
   PIPE_NODE = 6

};
class QueryNode : public TableProducer {
   public:
   virtual ~QueryNode() override = default;

   explicit QueryNode(QueryNodeType type) : TableProducer(NodeType::QUERY_NODE), type(type){};

   //! The type of the query node, either SetOperation or Select
   QueryNodeType type;

   /// The set of result modifiers associated with this query node
   std::vector<std::shared_ptr<ResultModifier>> modifiers{};

   std::shared_ptr<TableProducer> input;

   virtual std::string toString(uint32_t depth) = 0;

   virtual std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) = 0;
};
} // namespace lingodb::ast
