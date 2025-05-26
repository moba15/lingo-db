#pragma once
#include "lingodb/compiler/frontend/sql-parser/table_producer.h"
#include "lingodb/compiler/frontend/sql-parser/tableref.h"
namespace lingodb::ast {
class BoundTableRef : public TableProducer {
   public:
   explicit BoundTableRef(TableReferenceType type) : TableProducer(NodeType::BOUND_TABLE_REF), type(type) {
   }
   TableReferenceType type;

   virtual std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) = 0;
};

class BoundBaseTableRef : public BoundTableRef {
   public:
   static constexpr TableReferenceType TYPE = TableReferenceType::BASE_TABLE;
   BoundBaseTableRef(std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry);

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;

   //TODO missing semantic
   std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry;
};
} // namespace lingodb::ast