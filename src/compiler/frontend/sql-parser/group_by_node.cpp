#include "lingodb/compiler/frontend/sql-parser/group_by_node.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
std::string GroupByNode::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix
   ast.append("GroupByNode $1: group_expressions\n");
   for (auto& expression : group_expressions) {
      ast.append(expression->toAsciiAST(depth+1));
   }
   return ast;
}
} // namespace lingodb::ast