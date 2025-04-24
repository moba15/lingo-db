#include "lingodb/compiler/frontend/sql-parser/parsed_expression/columnref_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"

#include <cassert>
namespace lingodb::ast {
//TODO Find better solution for ColumnRefExpression than duckdb does with columnName and tableName
ColumnRefExpression::ColumnRefExpression(std::string columnName, std::string tableName)
  : ColumnRefExpression(tableName.empty() ? std::vector<std::string>{std::move(columnName)}
                                          : std::vector<std::string>{std::move(tableName), std::move(columnName)}){




}
ColumnRefExpression::ColumnRefExpression(std::string columnName) : ColumnRefExpression(std::vector<std::string>{std::move(columnName)}){

}

ColumnRefExpression::ColumnRefExpression(std::vector<std::string> columnNames) : ParsedExpression(ExpressionType::COLUMN_REF, TYPE), column_names(columnNames) {
   for (auto& columnName : columnNames) {
      assert(!columnName.empty());
   }

}

std::string ColumnRefExpression::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix
   ast.append("ColumnRefExpression: [");
   for (auto& columnName : column_names) {
      ast.append(columnName);
      ast.append(",");
   }
   ast = ast.substr(0, ast.size() - 1);
   ast.append("]");
   ast.append("\n");
   return ast;
}
} // namespace lingodb::ast