#pragma once
#include "tabledescription.h"
#include "lingodb/compiler/frontend/sql-parser/tableref.h"

#include <string>
namespace lingodb::ast {
class BaseTableRef : public TableRef {
public:
   static constexpr TableReferenceType TYPE = TableReferenceType::BASE_TABLE;
   BaseTableRef(TableDescription tableDescription);



   //! The catalog name.
   std::string catalogName;
   //! The schema name.
   std::string schemaName;
   //! The table name.
   std::string tableName;
   //! The timestamp/version at which to read this table entry (if any)
 //TODO  unique_ptr<AtClause> at_clause;

   std::string toAsciiAST(uint32_t depth ) override;

};
} // namespace lingodb::ast