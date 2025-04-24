#include "lingodb/compiler/frontend/sql-parser/parsed_expression/constant_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"

#include <string>
namespace lingodb::ast {
ConstantExpression::ConstantExpression(int iVal) :ParsedExpression(ExpressionType::VALUE_CONSTANT, TYPE), iVal(iVal) {}
ConstantExpression::ConstantExpression(std::string) :ParsedExpression(ExpressionType::VALUE_CONSTANT, TYPE),iVal(0) {

}
ConstantExpression::ConstantExpression() :ParsedExpression(ExpressionType::VALUE_CONSTANT, TYPE),iVal(0) {

}

std::string ConstantExpression::toAsciiAST(uint32_t depth ) {
   toAsciiASTPrefix
   ast.append("ConstantExpression: ");
   ast.append(std::to_string(iVal));
   ast.append("\n");

   return ast;
}
} // namespace lingodb::ast

