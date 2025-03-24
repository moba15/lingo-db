#include "lingodb/server/ipc/IPCHelper.h"
namespace server {
namespace util {
arrow::Result<std::shared_ptr<arrow::Buffer>> serializeTable(const std::shared_ptr<arrow::Table>& table) {
   std::shared_ptr<arrow::io::BufferOutputStream> buffer_stream =
      arrow::io::BufferOutputStream::Create().ValueOrDie();
   ARROW_ASSIGN_OR_RAISE(auto writer2, arrow::ipc::MakeStreamWriter(buffer_stream, table->schema()));
   std::shared_ptr<arrow::TableBatchReader> table_reader = std::make_shared<arrow::TableBatchReader>(*table);
   std::shared_ptr<arrow::RecordBatch> batch;
   while (table_reader->ReadNext(&batch).ok() && batch != nullptr) {
      ARROW_RETURN_NOT_OK(writer2->WriteRecordBatch(*batch));
   }
   ARROW_RETURN_NOT_OK(writer2->Close());
   ARROW_ASSIGN_OR_RAISE(auto buffer2, buffer_stream->Finish())
   return std::move(buffer2);
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> deserializeTableFromBufferToStream(std::shared_ptr<arrow::Buffer> buffer) {
   if (buffer == nullptr) { return arrow::Status::Invalid("Buffer is null"); }

   auto reader = std::make_shared<arrow::io::BufferReader>(buffer);
   ARROW_ASSIGN_OR_RAISE(auto recordBatchReader, arrow::ipc::RecordBatchStreamReader::Open(reader))

   auto stream = std::unique_ptr<arrow::flight::FlightDataStream>(
      std::make_unique<arrow::flight::RecordBatchStream>(recordBatchReader));

   return arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>(std::move(stream));
}
}
}; //namespace util
