#include <iomanip>
#include <iostream>
#include <sstream>

#include <arrow/pretty_print.h>
#include <arrow/table.h>

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
   options.container_window = 100;
   std::cout << "|";
   std::string rowSep = "-";
   std::vector<bool> convertHex;

   for (auto c : table->columns()) {
      auto typeId = table->schema()->field(positions.size())->type()->id();
      std::cout << std::setw(30) << table->schema()->field(positions.size())->name() << "  |";
      convertHex.push_back(typeId == arrow::Type::FIXED_SIZE_BINARY);
      rowSep += std::string(33, '-');
      std::string str;
      arrow::PrettyPrint(*c.get(), options, &str); //NOLINT (clang-diagnostic-unused-result)
      columnReps.push_back(str);
      positions.push_back(0);
   }
   std::cout << std::endl
             << rowSep << std::endl;

   // Track the nesting depth of brackets per column
   std::vector<int> bracketDepth(columnReps.size(), 0);

   bool cont = true;
   while (cont) {
      cont = false;

      // Buffer to hold exactly one horizontal row of cells
      std::vector<std::string> rowCells(columnReps.size());
      bool rowHasData = false;

      for (size_t column = 0; column < columnReps.size(); column++) {
         char32_t currChar = U'\0';
         uint8_t currCharSize = 0;
         std::stringstream out;
         bool cellHasData = false;

         if (positions[column] < columnReps[column].size()) {
            cont = true;
         }

         while (positions[column] < columnReps[column].size()) {
            char curr = columnReps[column][positions[column]];

            // 1. Bracket tracking
            if (curr == '[') bracketDepth[column]++;
            int depth = bracketDepth[column];
            if (curr == ']') bracketDepth[column]--;
            positions[column]++;

            // 2. Skip leading structural junk (brackets, separators, spaces at depth 1 or 2)
            if (!cellHasData && (depth <= 2) && (curr == '[' || curr == ']' || curr == ',' || curr == ' ' || curr == '\n')) {
               continue;
            }

            // 3. Handle structural commas (comma followed by newline at depth 1 or 2)
            if (cellHasData && (depth <= 2) && curr == ',') {
               char next = (positions[column] < columnReps[column].size()) ? columnReps[column][positions[column]] : '\0';
               if (next == '\n') {
                  continue;
               }
            }

            // 4. Handle end of cell
            if (cellHasData && (depth <= 2) && curr == '\n') {
               break;
            }

            // 5. Value Processing
            cellHasData = true;
            if (curr == '\n') {
               // Inner newline (depth > 2), skip leading spaces of next line and replace with space
               while (positions[column] < columnReps[column].size() && columnReps[column][positions[column]] == ' ') {
                  positions[column]++;
               }
               out << ' ';
               continue;
            }

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

         // Final flush for the cell
         if (currChar != U'\0') {
            for (size_t i = 0; i < currCharSize / 2; i++) {
               const char slice = reinterpret_cast<char*>(&currChar)[i];
               if (slice != 0) {
                  out << slice;
               }
            }
         }

         // Store the extracted text into the buffer
         rowCells[column] = out.str();
         if (cellHasData) {
            rowHasData = true;
         }
      }

      // 6. Print the completely synchronized row
      if (rowHasData) {
         std::cout << "|";
         for (size_t column = 0; column < columnReps.size(); column++) {
            std::cout << std::setw(30) << rowCells[column] << "  |";
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
