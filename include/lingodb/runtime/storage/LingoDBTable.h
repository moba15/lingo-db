#ifndef LINGODB_RUNTIME_STORAGE_LINGODBTABLE_H
#define LINGODB_RUNTIME_STORAGE_LINGODBTABLE_H
#include "TableStorage.h"
#include "lingodb/catalog/TableCatalogEntry.h"

#include <functional>
#include <string>
namespace lingodb::runtime {

class LingoDBTable : public TableStorage {
   bool persist;
   std::string fileName;
   std::string dbDir;
   catalog::Sample sample;
   std::shared_ptr<arrow::Schema> schema;
   std::vector<std::shared_ptr<arrow::RecordBatch>> tableData;
   std::unordered_map<std::string, catalog::ColumnStatistics> columnStatistics;
   size_t numRows;
   bool loaded = false;
   //todo: somehow we must be aware of the indices that are built on this table, and update them...
   public:
   LingoDBTable(std::string fileName, std::shared_ptr<arrow::Schema> schema);
   LingoDBTable(std::string fileName, std::shared_ptr<arrow::Schema> schema, size_t numRows, catalog::Sample sample, std::unordered_map<std::string, catalog::ColumnStatistics> columnStatistics) : persist(false), fileName(std::move(fileName)), sample(std::move(sample)), schema(std::move(schema)), columnStatistics(std::move(columnStatistics)), numRows(numRows) {}
   void setPersist(bool persist) {
      this->persist = persist;
      if (persist) {
         flush();
      }
   }
   size_t nextRowId() override {
      return numRows;
   }
   size_t getColIndex(std::string colName);
   std::unique_ptr<scheduler::Task> createScanTask(const ScanConfig& scanConfig) override;
   const catalog::Sample& getSample() const {
      return sample;
   }
   const catalog::ColumnStatistics& getColumnStatistics(std::string column) const;
   size_t getNumRows() const {
      return numRows;
   }
   ~LingoDBTable() = default;

   //flushes the data to disk
   void flush();
   //ensures that the data is loaded
   void ensureLoaded();
   virtual void setDBDir(std::string dbDir) {
      this->dbDir = dbDir;
   };

   void serialize(lingodb::utility::Serializer& serializer) const;
   static std::unique_ptr<LingoDBTable> deserialize(lingodb::utility::Deserializer& deserializer);
   void append(const std::vector<std::shared_ptr<arrow::RecordBatch>>& toAppend) override;
   void append(const std::shared_ptr<arrow::Table>& toAppend) override;
   static std::unique_ptr<LingoDBTable> create(const catalog::CreateTableDef& def);
   std::pair<std::shared_ptr<arrow::RecordBatch>, size_t> getByRowId(size_t rowId) const override;

   std::shared_ptr<arrow::DataType> getColumnStorageType(const std::string& columnName) const override;
};
} // namespace lingodb::runtime

#endif //LINGODB_RUNTIME_STORAGE_LINGODBTABLE_H
