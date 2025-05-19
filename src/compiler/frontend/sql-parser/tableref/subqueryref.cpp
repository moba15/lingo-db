#include "lingodb/compiler/frontend/sql-parser/tableref/subqueryref.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
SubqueryRef::SubqueryRef(std::shared_ptr<QueryNode> subSelectNode) : TableRef(TYPE), subSelectNode(std::move(subSelectNode)) {
}

std::string SubqueryRef::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot{};

   // Create node identifier for the SELECT node
   std::string nodeId = "node" + std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this)));
   dot += nodeId + " [label=\"Subquery\"];\n";

   // Handle select list
   if (subSelectNode) {
      std::string selectListId = "node" + std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(subSelectNode.get())));
      dot += nodeId + " -> " + selectListId + " [label=\"\"];\n";
      dot += subSelectNode->toDotGraph(depth + 1, idGen);
   }

   return dot;
}

} // namespace lingodb::ast