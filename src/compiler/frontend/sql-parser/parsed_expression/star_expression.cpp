#include "lingodb/compiler/frontend/sql-parser/parsed_expression/star_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
StarExpression::StarExpression(std::string relationName)
   : ParsedExpression(ExpressionType::STAR, ExpressionClass::STAR), relationName(std::move(relationName)) {
}

std::string StarExpression::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix return "";
}
std::string StarExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot{};

   // Create node identifier for the star expression
   std::string nodeId;
   nodeId.append("node");
   nodeId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))));

   // Create the star node with its label
   dot.append(nodeId);
   dot.append(" [label=\"");
   if (!relationName.empty()) {
      dot.append(relationName);
      dot.append(".");
   }
   dot.append("*");
   if (columnsExpr) {
      dot.append("\\nCOLUMNS");
   }
   dot.append("\"];\n");

   // Add expression if present
   if (expr) {
      std::string exprId;
      exprId.append("node");
      exprId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(expr.get()))));

      dot.append(nodeId);
      dot.append(" -> ");
      dot.append(exprId);
      dot.append(" [label=\"expr\"];\n");
      dot.append(expr->toDotGraph(depth + 1, idGen));
   }

   return dot;
}
} // namespace lingodb::ast