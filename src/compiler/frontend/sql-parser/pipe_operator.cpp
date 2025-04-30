#include "lingodb/compiler/frontend/sql-parser/pipe_operator.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
PipeOperator::PipeOperator(std::shared_ptr<AstNode> node) : AstNode(NodeType::PIPE_OP), node(node) {
}
bool PipeOperator::isLast() {
   return next == nullptr;
}
std::string PipeOperator::toAsciiAST(uint32_t depth) {
   std::string ast{};
   ast.append(node->toAsciiAST(depth));

   if (next)
      ast.append(next->toAsciiAST(depth));
   return ast;
}
std::string PipeOperator::toDotGraph(uint32_t depth) {

   std::string dot{};



   dot += "node" + std::to_string(reinterpret_cast<uintptr_t>(this)) +
          " [label=\"Pipe Operator\"];\n";

   dot += "node" + std::to_string(reinterpret_cast<uintptr_t>(this)) +
          " -> node" + std::to_string(reinterpret_cast<uintptr_t>(node.get())) + ";\n";

   dot += node->toDotGraph(depth + 1);



   return dot;
}

} // namespace lingodb::ast