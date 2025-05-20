#include "lingodb/compiler/frontend/sql-parser/pipe_operator.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
PipeOperator::PipeOperator(PipeOperatorType pipeOpType, std::shared_ptr<AstNode> node) : TableProducer(NodeType::PIPE_OP), node(node), pipeOpType(pipeOpType) {
}
bool PipeOperator::isLast() {
   return input == nullptr;
}

std::string PipeOperator::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot{};

   dot.append(inputToDotGraph(depth, idGen));

   dot += "node" + std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))) +
      " [label=\"Pipe Operator\"];\n";

   dot += "node" + std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))) +
      " -> node" + std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(node.get()))) + ";\n";

   dot += node->toDotGraph(depth + 1, idGen);

   return dot;
}

} // namespace lingodb::ast