#pragma once

#include <iostream>
#include <memory>
#include <arrow/api.h>
#include <arrow/flight/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
namespace server {
namespace util {
arrow::Result<std::shared_ptr<arrow::Buffer>> serializeTable(const std::shared_ptr<arrow::Table>& table);
arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> deserializeTableFromBufferToStream(std::shared_ptr<arrow::Buffer> buffer);
}; //namespace util
}

