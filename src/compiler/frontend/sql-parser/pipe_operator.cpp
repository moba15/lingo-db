#include "lingodb/compiler/frontend/sql-parser/pipe_operator.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
PipeOperator::PipeOperator(PipeOperatorType type, std::shared_ptr<AstNode> node) : AstNode(NodeType::PIPE_OP), node(node), type(type) {
}
bool PipeOperator::isLast() {
   return next == nullptr;
}

   if (next)
      ast.append(next->toAsciiAST(depth));
   return ast;
}
std::string PipeOperator::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot{};

   dot += "node" + std::to_string(reinterpret_cast<uintptr_t>(this)) +
      " [label=\"Pipe Operator\"];\n";

   dot += "node" + std::to_string(reinterpret_cast<uintptr_t>(this)) +
      " -> node" + std::to_string(reinterpret_cast<uintptr_t>(node.get())) + ";\n";

   dot += node->toDotGraph(depth + 1, idGen);

   return dot;
}

} // namespace lingodb::ast