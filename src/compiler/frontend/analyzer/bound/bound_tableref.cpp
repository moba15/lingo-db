#include "lingodb/compiler/frontend/analyzer/bound/bound_tableref.h"
namespace lingodb::ast {
/*
 * BoundBase Table
*/
BoundBaseTableRef::BoundBaseTableRef(std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry) : BoundTableRef(TYPE), tableCatalogEntry(std::move(tableCatalogEntry)) {
}
BoundBaseTableRef::BoundBaseTableRef(std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry, std::string alias) : BoundTableRef(TYPE, std::move(alias)), tableCatalogEntry(std::move(tableCatalogEntry)) {
}
std::string BoundBaseTableRef::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot;

   // Create node identifier for the base table reference
   std::string nodeId = "node" + std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this)));

   // Create label with table information
   std::string label = "BoundBaseTable\\n table: " + tableCatalogEntry->getName() + "\\n" +
      "tableCategoryEntry" + tableCatalogEntry->getName();

   //TODO
   // Add alias if it's not empty

   // Create the node with all information
   dot += nodeId + " [label=\"" + label + "\"];\n";

   return dot;
}

BoundJoinRef::BoundJoinRef(JoinType type, JoinCondType refType, std::shared_ptr<TableProducer> left, std::shared_ptr<TableProducer> right, boundJoinCond condition) : BoundTableRef(TYPE), type(type), refType(refType), left(std::move(left)), right(std::move(right)), condition(std::move(condition)) {

}
std::string BoundJoinRef::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}

BoundSubqueryRef::BoundSubqueryRef(std::shared_ptr<analyzer::SQLScope> sqlScope, std::shared_ptr<TableProducer> subSelect) : BoundTableRef(TYPE), sqlScope(sqlScope), subSelect(subSelect){
}
std::string BoundSubqueryRef::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}
} // namespace lingodb::ast