#include "lingodb/compiler/frontend/sql-parser/tableref/basetableref.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
 BaseTableRef::BaseTableRef(TableDescription tableDescription) : TableRef(TYPE),  catalogName(tableDescription.database), schemaName(tableDescription.schema), tableName(tableDescription.table) {
}

std::string BaseTableRef::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix
      ast.append("BaseTableRef: (table: ");
   ast.append(tableName);
   ast.append(" , schema: ");
   ast.append(schemaName);
   ast.append(" , catalog: ");
   ast.append(catalogName);
   ast.append(" , alias: ");
   ast.append(alias);

   ast.append(")\n");
   return ast;
}
std::string BaseTableRef::toDotGraph(uint32_t depth) {
    std::string dot{};
    
    // Create node identifier for the base table reference
    std::string nodeId = "node" + std::to_string(reinterpret_cast<uintptr_t>(this));
    
    // Create label with all table information
    std::string label = "BaseTable\\n table: " + tableName + "\\n" +
                       "schema: " + schemaName + "\\n" +
                       "catalog: " + catalogName;
    
    // Add alias if it's not empty
    if (!alias.empty()) {
        label += "\\nalias: " + alias;
    }
    
    // Create the node with all information
    dot += nodeId + " [label=\"" + label + "\"];\n";
    
    return dot;
}
} // namespace lingodb::ast