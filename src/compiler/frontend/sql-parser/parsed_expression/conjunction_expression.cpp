#include "lingodb/compiler/frontend/sql-parser/parsed_expression/conjunction_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {

ConjunctionExpression::ConjunctionExpression(ExpressionType type): ParsedExpression(type, TYPE), children() {}
ConjunctionExpression::ConjunctionExpression(ExpressionType type, std::shared_ptr<lingodb::ast::ParsedExpression> left, std::shared_ptr<lingodb::ast::ParsedExpression> right) : ConjunctionExpression(type, std::vector{left,right}) {}
 ConjunctionExpression::ConjunctionExpression(ExpressionType type, std::vector<std::shared_ptr<ParsedExpression>> children) : ParsedExpression(type, TYPE), children(std::move(children)) {
}

std::string ConjunctionExpression::typeToAscii(ExpressionType type) const{
   switch (type) {
      case ExpressionType::CONJUNCTION_AND: return "AND";
      case ExpressionType::CONJUNCTION_OR: return "OR";
      default: return "Not found";
   }
}

std::string ConjunctionExpression::toAsciiAST(uint32_t depth )  {
   toAsciiASTPrefix
   ast.append("ConjunctionExpression: ");
   ast.append(typeToAscii(type));
   ast.append("\n");
   for (auto& child : children) {
      ast.append(child->toAsciiAST(depth+1));
   }
   return ast;
 };
}// namespace lingodb::ast