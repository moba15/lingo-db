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
std::string ColumnRefExpression::toDotGraph(uint32_t depth) {
    std::string dot{};
    
    // Create node identifier
    std::string nodeId;
    nodeId.append("node");
    nodeId.append(std::to_string(reinterpret_cast<uintptr_t>(this)));
    
    // Create the label with column names
    std::string label;
    label.append("ColumnRef\\n");
    
    // Add all column names with dots between them
    for (size_t i = 0; i < column_names.size(); ++i) {
        if (i > 0) {
            label.append(".");
        }
        label.append(column_names[i]);
    }
    
    // Create the node
    dot.append(nodeId);
    dot.append(" [label=\"");
    dot.append(label);
    dot.append("\"];\n");
    return dot;
}
} // namespace lingodb::ast