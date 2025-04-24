#include "lingodb/compiler/frontend/sql-parser/parsed_expression/function_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
FunctionExpression::FunctionExpression(std::string catalog, std::string schema, std::string functionName, bool isOperator, bool distinct, bool exportState) : ParsedExpression(ExpressionType::FUNCTION, TYPE), catalog(catalog), schema(schema), functionName(functionName), isOperator(isOperator), distinct(distinct), exportState(exportState) {}
std::string FunctionExpression::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix
   ast.append("FunctionExpression: ");
   ast.append(functionName);
   ast.append("\n");
   for (auto& argument: arguments) {
      ast.append(argument->toAsciiAST(depth + 1));
   }

   return ast;
}

} // namespace lingodb::ast