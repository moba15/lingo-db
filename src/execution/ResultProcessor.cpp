#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>

// Replaces <arrow/pretty_print.h> to give us access to arrays and scalars directly
#include <arrow/api.h>

#include "lingodb/execution/ResultProcessing.h"
#include "lingodb/runtime/ArrowTable.h"
#include <functional>

namespace {

class TableRetriever : public lingodb::execution::ResultProcessor {
   std::shared_ptr<arrow::Table>& result;

   public:
   TableRetriever(std::shared_ptr<arrow::Table>& result) : result(result) {}
   void process(lingodb::runtime::ExecutionContext* executionContext) override {
      auto resultTable = executionContext->getResultOfType<lingodb::runtime::ArrowTable>(0);
      if (!resultTable) return;
      result = resultTable.value()->get();
   }
};

void printTable(const std::shared_ptr<arrow::Table>& table) {
   // Do not output anything for insert or copy statements
   if (table->columns().empty()) {
      std::cout << "Statement executed successfully." << std::endl;
      return;
   }

   // 1. Print Table Header
   std::cout << "|";
   std::string rowSep = "-";
   for (int i = 0; i < table->num_columns(); ++i) {
      std::cout << std::setw(30) << table->schema()->field(i)->name() << "  |";
      rowSep += std::string(33, '-');
   }
   std::cout << "\n" << rowSep << "\n";

   // 2. Iterate Row-by-Row
   for (int64_t row = 0; row < table->num_rows(); ++row) {
      std::cout << "|";
      for (int col = 0; col < table->num_columns(); ++col) {
         auto chunked_arr = table->column(col);

         // Arrow Tables are chunked. Find the specific chunk that contains our target row.
         int64_t chunk_row = row;
         int chunk_idx = 0;
         while (chunk_idx < chunked_arr->num_chunks() && chunk_row >= chunked_arr->chunk(chunk_idx)->length()) {
            chunk_row -= chunked_arr->chunk(chunk_idx)->length();
            chunk_idx++;
         }

         std::string outStr = "";
         if (chunk_idx < chunked_arr->num_chunks()) {
            auto array = chunked_arr->chunk(chunk_idx);
            auto result = array->GetScalar(chunk_row);

            if (result.ok() && result.ValueOrDie()->is_valid) {
               auto scalar = result.ValueOrDie();

               // Bypass Arrow's formatting for binaries to output raw bytes directly
               // (This replaces your previous manual hex-to-char conversion loop)
               if (scalar->type->id() == arrow::Type::FIXED_SIZE_BINARY) {
                  auto bin_scalar = std::static_pointer_cast<arrow::FixedSizeBinaryScalar>(scalar);
                  outStr = std::string(reinterpret_cast<const char*>(bin_scalar->value->data()), bin_scalar->value->size());
               } else if (arrow::is_base_binary_like(scalar->type->id())) {
                  auto bin_scalar = std::static_pointer_cast<arrow::BaseBinaryScalar>(scalar);
                  outStr = std::string(reinterpret_cast<const char*>(bin_scalar->value->data()), bin_scalar->value->size());
               } else {
                  // For primitive types, Lists, and Structs, use Arrow's built-in string conversion
                  // A struct will beautifully format as: {"key1": val1, "key2": val2}
                  outStr = scalar->ToString();
               }
            } else {
               outStr = "NULL";
            }
         }

         // Prevent any stray newlines (from nested schemas) from breaking table alignment
         std::replace(outStr.begin(), outStr.end(), '\n', ' ');

         // Print cell with consistent padding
         std::cout << std::setw(30) << outStr << "  |";
      }
      std::cout << "\n";
   }
}

class TablePrinter : public lingodb::execution::ResultProcessor {
   void process(lingodb::runtime::ExecutionContext* executionContext) override {
      auto resultTable = executionContext->getResultOfType<lingodb::runtime::ArrowTable>(0);
      if (!resultTable) return;
      auto table = resultTable.value()->get();
      printTable(table);
   }
};

class BatchedTablePrinter : public lingodb::execution::ResultProcessor {
   void process(lingodb::runtime::ExecutionContext* executionContext) override {
      for (size_t i = 0;; i++) {
         auto resultTable = executionContext->getResultOfType<lingodb::runtime::ArrowTable>(i);
         if (!resultTable) return;
         auto table = resultTable.value()->get();
         printTable(table);
      }
   }
};

} // namespace

std::unique_ptr<lingodb::execution::ResultProcessor> lingodb::execution::createTableRetriever(std::shared_ptr<arrow::Table>& result) {
   return std::make_unique<TableRetriever>(result);
}

std::unique_ptr<lingodb::execution::ResultProcessor> lingodb::execution::createTablePrinter() {
   return std::make_unique<TablePrinter>();
}

std::unique_ptr<lingodb::execution::ResultProcessor> lingodb::execution::createBatchedTablePrinter() {
   return std::make_unique<BatchedTablePrinter>();
}