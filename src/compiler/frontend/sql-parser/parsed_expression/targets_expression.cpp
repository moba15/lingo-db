#include "lingodb/compiler/frontend/sql-parser/parsed_expression/targets_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
TargetsExpression::TargetsExpression() : ParsedExpression(ExpressionType::TARGETS,TYPE) {
}
std::string TargetsExpression::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix
   ast.append("TargetsList\n");
   for (auto& target : targets) {
      if (target)
         ast.append(target->toAsciiAST(depth + 1));
   }
   return ast;
}
} // namespace lingodb::ast