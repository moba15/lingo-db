#include "lingodb/compiler/frontend/sql-parser/query_node/pipe_select_node.h"

#include "lingodb/compiler/frontend/sql-parser/parsed_expression/targets_expression.h"
namespace lingodb::ast {
PipeSelectNode::PipeSelectNode() : QueryNode(TYPE){
}
PipeSelectNode::~PipeSelectNode() = default;
std::string PipeSelectNode::toString(uint32_t depth) {
   return "";
}
std::string PipeSelectNode::toAsciiAST(uint32_t depth) {

   toAsciiASTPrefix
   ast.append("PIPE:");
   ast.append("\n");

   if (startPipeOperator) {
      ast.append(startPipeOperator->toAsciiAST(depth+1));
   }


   return ast;
}
} // namespace lingodb::ast