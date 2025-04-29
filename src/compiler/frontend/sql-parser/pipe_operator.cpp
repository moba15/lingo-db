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
} // namespace lingodb::ast