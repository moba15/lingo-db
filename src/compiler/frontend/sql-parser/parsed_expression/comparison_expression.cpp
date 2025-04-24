#include "lingodb/compiler/frontend/sql-parser/parsed_expression/comparison_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
ComparisonExpression::ComparisonExpression(ExpressionType type) : ParsedExpression(type, TYPE)  {
}
ComparisonExpression::ComparisonExpression(ExpressionType type, std::shared_ptr<ParsedExpression> left, std::shared_ptr<ParsedExpression> right): ParsedExpression(type, TYPE), left(std::move(left)), right(std::move(right)) {

}


std::string ComparisonExpression::toAsciiAST(uint32_t depth ) {
   toAsciiASTPrefix

   ast.append("ComparisonExpression: $1 ");
   ast.append(typeToAscii(type));
   ast.append(" $2\n");
   if (left)
      ast.append(left->toAsciiAST(depth+1));
   if (right)
      ast.append(right->toAsciiAST(depth+1));

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

} // namespace lingodb::ast