//
// Created by mor on 20.11.24.
//

#include "server/ipc/IPCHelper.h"
namespace server  {
namespace util {
arrow::Result<std::shared_ptr<arrow::ResizableBuffer>> serializeTable(const std::shared_ptr<arrow::Table>& table) {
   ARROW_ASSIGN_OR_RAISE(std::shared_ptr<arrow::ResizableBuffer> buffer, arrow::AllocateResizableBuffer(1024 * 1024));
   std::shared_ptr<arrow::ResizableBuffer> buffer2;
   auto stream = std::make_shared<arrow::io::FixedSizeBufferWriter>(buffer);
   ARROW_ASSIGN_OR_RAISE(auto writer, arrow::ipc::MakeStreamWriter(stream, table->schema()));
   if (!writer->WriteTable(*table).ok() || !writer->Close().ok()) {
      std::cerr << "Failed to write table to buffer" << std::endl;
      return arrow::Status::Invalid("Failed to write table to buffer");
   }
   ARROW_RETURN_NOT_OK(stream->Close());
   return arrow::Result<std::shared_ptr<arrow::ResizableBuffer>>(std::move(buffer));
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>
deserializeTableFromBufferToStream(std::shared_ptr<arrow::Buffer> buffer) {
   if (buffer == nullptr) { return arrow::Status::Invalid("Buffer is null"); }

   auto reader = std::make_shared<arrow::io::BufferReader>(buffer);
   ARROW_ASSIGN_OR_RAISE(auto recordBatchReader, arrow::ipc::RecordBatchStreamReader::Open(reader))

   auto stream = std::unique_ptr<arrow::flight::FlightDataStream>(
      std::make_unique<arrow::flight::RecordBatchStream>(recordBatchReader));

   return arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>>(std::move(stream));
}
}
}; //namespace util
