#include "lingodb/compiler/frontend/sql-parser/query_node/pipe_select_node.h"

#include "lingodb/compiler/frontend/sql-parser/parsed_expression/targets_expression.h"
namespace lingodb::ast {
PipeSelectNode::PipeSelectNode() : QueryNode(TYPE) {
}
PipeSelectNode::~PipeSelectNode() = default;

std::string PipeSelectNode::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot{};

   // Create node identifier for the pipe select node
   std::string nodeId;
   nodeId.append("node");
   nodeId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))));

   // Create the pipe select node with its label
   dot.append(nodeId);
   dot.append(" [label=\"PIPE SELECT NODE\"];\n");

   std::shared_ptr<PipeOperator> currentOp = startPipeOperator;
   size_t i = 0;
   while (currentOp) {
      dot.append(currentOp->node->toDotGraph(depth + 1, idGen));
      std::string startOpId;
      startOpId.append("node");
      startOpId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(currentOp->node.get()))));

      // Create edge from pipe select to start operator
      dot.append(nodeId);
      dot.append(" -> ");
      dot.append(startOpId);
      if (i > 0)
         dot.append(" [label=\"next\"];\n");
      else
         dot.append(" [label=\"start\"];\n");

      nodeId.clear();
      nodeId.append("node");
      nodeId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(currentOp->node.get()))));

      currentOp = currentOp->next;
      i++;
   }

   return dot;
}
} // namespace lingodb::ast