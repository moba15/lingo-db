#include "lingodb/compiler/frontend/sql-parser/parsed_expression/targets_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
TargetsExpression::TargetsExpression() : ParsedExpression(ExpressionType::TARGETS, TYPE) {
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
std::string TargetsExpression::toDotGraph(uint32_t depth) {
   std::string dot{};

   // Create node identifier for the targets list
   std::string nodeId;
   nodeId.append("node");
   nodeId.append(std::to_string(reinterpret_cast<uintptr_t>(this)));

   // Create the targets list node
   dot.append(nodeId);
   dot.append(" [label=\"Targets List\"];\n");

   // Add all target expressions
   for (size_t i = 0; i < targets.size(); ++i) {
      if (targets[i]) {
         std::string targetId;
         targetId.append("node");
         targetId.append(std::to_string(reinterpret_cast<uintptr_t>(targets[i].get())));

         // Create edge from targets list to this target
         dot.append(nodeId);
         dot.append(" -> ");
         dot.append(targetId);
         dot.append(" [label=\"target ");
         dot.append(std::to_string(i + 1));
         dot.append("\"];\n");

         // Add the target's graph representation
         dot.append(targets[i]->toDotGraph(depth + 1));
      }
   }

   return dot;
}
} // namespace lingodb::ast