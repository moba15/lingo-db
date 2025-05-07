#pragma once
#include "lingodb/catalog/Catalog.h"
#include "lingodb/catalog/TableCatalogEntry.h"
#include "lingodb/compiler/frontend/sql-parser/tableref.h"
#include "tabledescription.h"

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

   /*
    * Semantic
   */
   std::shared_ptr<catalog::TableCatalogEntry> catalogEntry = nullptr;
   std::string scopeName;

   std::string toAsciiAST(uint32_t depth) override;
   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
} // namespace lingodb::ast