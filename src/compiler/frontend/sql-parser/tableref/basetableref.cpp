#include "lingodb/compiler/frontend/sql-parser/tableref/basetableref.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
 BaseTableRef::BaseTableRef(TableDescription tableDescription) : TableRef(TYPE),  catalogName(tableDescription.database), schemaName(tableDescription.schema), tableName(tableDescription.table) {
}

std::string BaseTableRef::toAsciiAST(uint32_t depth){
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
} // namespace lingodb::ast
