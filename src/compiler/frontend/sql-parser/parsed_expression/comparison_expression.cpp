#include "lingodb/compiler/frontend/sql-parser/parsed_expression/comparison_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
ComparisonExpression::ComparisonExpression(ExpressionType type) : ParsedExpression(type, TYPE) {
}
ComparisonExpression::ComparisonExpression(ExpressionType type, std::shared_ptr<ParsedExpression> left, std::shared_ptr<ParsedExpression> right) : ParsedExpression(type, TYPE), left(std::move(left)), right(std::move(right)) {
}

std::string ComparisonExpression::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix

      ast.append("ComparisonExpression: $1 ");
   ast.append(typeToAscii(type));
   ast.append(" $2\n");
   if (left)
      ast.append(left->toAsciiAST(depth + 1));
   if (right)
      ast.append(right->toAsciiAST(depth + 1));

   return ast;
}

std::string ComparisonExpression::typeToAscii(ExpressionType type) const {
   switch (type) {
      case ExpressionType::COMPARE_EQUAL: return "=";
      case ExpressionType::COMPARE_GREATERTHAN: return ">";
      case ExpressionType::COMPARE_LESSTHAN: return "<";
      case ExpressionType::COMPARE_GREATERTHANOREQUALTO: return ">=";
      case ExpressionType::COMPARE_LESSTHANOREQUALTO: return "<=";
      case ExpressionType::COMPARE_NOTEQUAL: return "<>";
      default: "Unknown";
   }
}
std::string ComparisonExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot{};

   // Create node identifier for the comparison expression
   std::string nodeId;
   nodeId.append("node");
   nodeId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))));

   // Create the node with operator label
   dot.append(nodeId);
   dot.append(" [label=\"σ\n");
   dot.append(typeToAscii(type));
   dot.append("\"];\n");

   // Handle left operand
   if (left) {
      std::string leftId;
      leftId.append("node");
      leftId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(left.get()))));

      dot.append(nodeId);
      dot.append(" -> ");
      dot.append(leftId);
      dot.append(" [label=\"left\"];\n");
      dot.append(left->toDotGraph(depth + 1, idGen));
   }

   // Handle right operand
   if (right) {
      std::string rightId;
      rightId.append("node");
      rightId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(right.get()))));

      dot.append(nodeId);
      dot.append(" -> ");
      dot.append(rightId);
      dot.append(" [label=\"right\"];\n");
      dot.append(right->toDotGraph(depth + 1, idGen));
   }

   return dot;
}

} // namespace lingodb::ast