#include "runtime/ArrowTable.h"
#include <arrow/table.h>
#include <utility/Tracer.h>
using namespace runtime;
namespace {
  utility::Tracer::Event tableMerge("ArrowTable", "merge");
} // end namespace
ArrowTable* ArrowTable::createEmpty() {
   auto table = arrow::Table::MakeEmpty(std::make_shared<arrow::Schema>(std::vector<std::shared_ptr<arrow::Field>>{})).ValueOrDie();
   return new ArrowTable(table);
}
ArrowTable* ArrowTable::addColumn(VarLen32 name, ArrowColumn* column) {
   auto fields = this->table->schema()->fields();
   fields.push_back(arrow::field(name, column->getColumn()->type()));
   auto schema = std::make_shared<arrow::Schema>(fields);
   auto arrays = this->table->columns();
   arrays.push_back(column->getColumn());
   return new ArrowTable(arrow::Table::Make(schema, arrays));
}
ArrowTable* ArrowTable::merge(ThreadLocal* threadLocal) {
   utility::Tracer::Trace trace(tableMerge);
   std::vector<std::shared_ptr<arrow::Table>> tables;
   ArrowTable* first = nullptr;
   for (auto* ptr : threadLocal->getTls()) {
      auto* current = reinterpret_cast<ArrowTable*>(ptr);
      tables.push_back(current->get());
   }
   auto concatenated = arrow::ConcatenateTables(tables).ValueOrDie();
   trace.stop();
   return new ArrowTable(concatenated);
}
