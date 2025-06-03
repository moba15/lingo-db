#pragma once
#include "lingodb/compiler/frontend/sql-parser/table_producer.h"
#include "lingodb/compiler/frontend/sql-parser/tableref.h"
namespace lingodb::ast {
class BoundTableRef : public TableProducer {
   public:
   explicit BoundTableRef(TableReferenceType type) : TableProducer(NodeType::BOUND_TABLE_REF), type(type) {
   }
   BoundTableRef(TableReferenceType type, std::string alias) : TableProducer(NodeType::BOUND_TABLE_REF, alias), type(type) {
   }
   TableReferenceType type;

   virtual std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) = 0;
};

class BoundBaseTableRef : public BoundTableRef {
   public:
   static constexpr TableReferenceType TYPE = TableReferenceType::BASE_TABLE;
   BoundBaseTableRef(std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry);
   BoundBaseTableRef(std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry, std::string alias);

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;

   //TODO missing semantic
   std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry;
};

class BoundJoinRef : public BoundTableRef {
   static constexpr TableReferenceType TYPE = TableReferenceType::JOIN;

   public:
   BoundJoinRef(JoinType type, JoinCondType refType, std::shared_ptr<TableProducer> left, std::shared_ptr<TableProducer> right, jointCondOrUsingCols condition);

   //! The left hand side of the join
   //! QueryNode as variant is needed for pipe syntax. Example: FROM Test |> join ok on id1=id2
   //! Bound left hand
   std::shared_ptr<TableProducer> left;
   //! Bound right hand
   std::shared_ptr<TableProducer> right;
   //TODO is this condition a good solution for on condiotion and using?
   //! The joint condition or a vector of ColumnRefExpression if USING
   jointCondOrUsingCols condition;
   //! The join type
   JoinType type;
   //! Join condition type
   JoinCondType refType;

   //! The set of USING columns (if any)
   //std::vector<std::string> usingColumns;
   /*//! Duplicate eliminated columns (if any)
   vector<unique_ptr<ParsedExpression>> duplicate_eliminated_columns;*/

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
} // namespace lingodb::ast