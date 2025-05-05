#include "lingodb/compiler/frontend/sql-parser/parsed_expression/constant_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"

#include <string>
namespace lingodb::ast {
ConstantExpression::ConstantExpression(int iVal) : ParsedExpression(ExpressionType::VALUE_CONSTANT, TYPE), iVal(iVal) {}
ConstantExpression::ConstantExpression(std::string) : ParsedExpression(ExpressionType::VALUE_CONSTANT, TYPE), iVal(0) {
}
ConstantExpression::ConstantExpression() : ParsedExpression(ExpressionType::VALUE_CONSTANT, TYPE), iVal(0) {
}

std::string ConstantExpression::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix
      ast.append("ConstantExpression: ");
   ast.append(std::to_string(iVal));
   ast.append("\n");

   return ast;
}
std::string ConstantExpression::toDotGraph(uint32_t depth,  NodeIdGenerator& idGen) {
   std::string dot{};

   // Create node identifier for the constant
   std::string nodeId;
   nodeId.append("node");
   nodeId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))));

   // Create label with constant value
   std::string label;
   label.append("Constant\\n");
   label.append(std::to_string(iVal));

   // Create the node
   dot.append(nodeId);
   dot.append(" [label=\"");
   dot.append(label);
   dot.append("\"];\n");

   return dot;
}
} // namespace lingodb::ast