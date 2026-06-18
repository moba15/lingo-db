#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <arrow/array.h>
#include <arrow/pretty_print.h>
#include <arrow/scalar.h>
#include <arrow/table.h>
#include <arrow/type.h>

#include "lingodb/execution/ResultProcessing.h"
#include "lingodb/runtime/ArrowTable.h"
#include <functional>

namespace {
unsigned char hexval(unsigned char c) {
   if ('0' <= c && c <= '9')
      return c - '0';
   else if ('a' <= c && c <= 'f')
      return c - 'a' + 10;
   else if ('A' <= c && c <= 'F')
      return c - 'A' + 10;
   else
      abort();
}

// Helper to safely escape strings for JSON
std::string escapeString(const std::string& str) {
   std::string escaped;
   for (char c : str) {
      if (c == '\n') escaped += "\\n";
      else if (c == '"') escaped += "\\\"";
      else escaped += c;
   }
   return escaped;
}

// Recursively convert Arrow Scalars to JSON strings
std::string scalarToJson(const std::shared_ptr<arrow::Scalar>& scalar) {
   if (!scalar || !scalar->is_valid) return "null";

   switch (scalar->type->id()) {
      case arrow::Type::STRUCT: {
         auto struct_scalar = std::static_pointer_cast<arrow::StructScalar>(scalar);
         auto struct_type = std::static_pointer_cast<arrow::StructType>(struct_scalar->type);
         std::stringstream ss;
         ss << "{";
         for (size_t i = 0; i < struct_scalar->value.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << "\"" << escapeString(struct_type->field(i)->name()) << "\": "
               << scalarToJson(struct_scalar->value[i]);
         }
         ss << "}";
         return ss.str();
      }
      case arrow::Type::LIST:
      case arrow::Type::LARGE_LIST:
      case arrow::Type::FIXED_SIZE_LIST: {
         auto list_scalar = std::static_pointer_cast<arrow::BaseListScalar>(scalar);
         std::stringstream ss;
         ss << "[";
         auto list_arr = list_scalar->value;
         for (int64_t i = 0; i < list_arr->length(); ++i) {
            if (i > 0) ss << ", ";
            auto item_scalar = list_arr->GetScalar(i);
            if (item_scalar.ok()) {
               ss << scalarToJson(item_scalar.ValueOrDie());
            } else {
               ss << "null";
            }
         }
         ss << "]";
         return ss.str();
      }
      case arrow::Type::STRING:
      case arrow::Type::LARGE_STRING: {
         auto str_scalar = std::static_pointer_cast<arrow::StringScalar>(scalar);
         return "\"" + escapeString(str_scalar->ToString()) + "\"";
      }
      default:
         return scalar->ToString();
   }
}

// Generate parser-friendly string mimicking Arrow's exact ChunkedArray formatting
std::string formatComplexColumn(const std::shared_ptr<arrow::ChunkedArray>& column) {
   std::stringstream ss;
   ss << "[\n";
   for (int i = 0; i < column->num_chunks(); ++i) {
      if (i > 0) ss << ",\n";
      ss << "[\n"; // Arrow chunks have inner brackets
      auto chunk = column->chunk(i);
      for (int64_t row = 0; row < chunk->length(); ++row) {
         if (row > 0) ss << ",\n";
         auto scalar_res = chunk->GetScalar(row);
         if (scalar_res.ok()) {
            ss << scalarToJson(scalar_res.ValueOrDie());
         } else {
            ss << "null";
         }
      }
      ss << "\n]";
   }
   ss << "\n]";
   return ss.str();
}

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

   std::vector<std::string> columnReps;
   std::vector<size_t> positions;
   arrow::PrettyPrintOptions options;
   options.indent_size = 0;
   options.window = 100;
   options.element_size_limit = 10000;
   std::cout << "|";
   std::string rowSep = "-";
   std::vector<bool> convertHex;

   for (auto c : table->columns()) {
      auto field = table->schema()->field(positions.size());
      std::cout << std::setw(30) << field->name() << "  |";
      convertHex.push_back(field->type()->id() == arrow::Type::FIXED_SIZE_BINARY);
      rowSep += std::string(33, '-');

      std::string str;
      auto type_id = field->type()->id();

      // Intercept struct and list types to format them into single-line JSON strings
      if (type_id == arrow::Type::STRUCT || type_id == arrow::Type::LIST ||
          type_id == arrow::Type::LARGE_LIST || type_id == arrow::Type::FIXED_SIZE_LIST) {
         str = formatComplexColumn(c);
      } else {
         arrow::PrettyPrint(*c.get(), options, &str); //NOLINT (clang-diagnostic-unused-result)
      }

      columnReps.push_back(str);
      positions.push_back(0);
   }

   std::cout << std::endl
             << rowSep << std::endl;

   bool cont = true;
   while (cont) {
      cont = false;
      bool any_valid = false;
      std::vector<std::string> row_strings(columnReps.size(), "");

      for (size_t column = 0; column < columnReps.size(); column++) {
         char32_t currChar = U'\0';
         uint8_t currCharSize = 0;

         bool first = true;
         std::stringstream out;
         while (positions[column] < columnReps[column].size()) {
            cont = true;
            char curr = columnReps[column][positions[column]];
            char next = positions[column] + 1 < columnReps[column].size() ? columnReps[column][positions[column] + 1] : '\0';
            positions[column]++;
            if (first && (curr == '[' || curr == ']' || curr == ',')) {
               continue;
            }
            if (curr == ',' && next == '\n') {
               continue;
            }
            if (curr == '\n') {
               break;
            } else {
               first = false;
               if (convertHex[column] && isxdigit(curr)) {
                  if (currCharSize % 2 == 0)
                     currChar |= hexval(curr) << (currCharSize++ * 4 + 4);
                  else
                     currChar |= hexval(curr) << (currCharSize++ * 4 - 4);
               } else if ((curr & (1 << 7)) == (1 << 7)) {
                  const auto extendedCurr = static_cast<char32_t>(curr) & 0xFF;
                  currChar |= static_cast<char32_t>(extendedCurr << (currCharSize * 4));
                  currCharSize += 2;
               } else {
                  if (currChar != U'\0') {
                     for (size_t i = 0; i < currCharSize / 2; i++) {
                        const char slice = reinterpret_cast<char*>(&currChar)[i];
                        if (slice != 0) {
                           out << slice;
                        }
                     }
                     currChar = U'\0';
                     currCharSize = 0;
                  }
                  out << curr;
               }
            }
         }
         if (currChar != U'\0') {
            for (size_t i = 0; i < currCharSize / 2; i++) {
               const char slice = reinterpret_cast<char*>(&currChar)[i];
               if (slice != 0) {
                  out << slice;
               }
            }
         }

         if (!first) {
            any_valid = true;
            row_strings[column] = out.str();
         }
      }

      if (any_valid) {
         std::cout << "|";
         for (size_t column = 0; column < columnReps.size(); column++) {
            std::cout << std::setw(30) << row_strings[column] << "  |";
         }
         std::cout << "\n";
      }
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