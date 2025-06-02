#include "lingodb/compiler/frontend/analyzer/sql_context.h"

#include "lingodb/runtime/RecordBatchInfo.h"

#include <iostream>
namespace lingodb::analyzer {
ASTTransformContext::ASTTransformContext() : aggregationNode(std::make_shared<ast::AggregationNode>()) {
}

void SQLContext::pushNewScope() {
   auto newScope = std::make_shared<SQLScope>();
   newScope->parent = currentScope;
   currentScope = newScope;
   scopes.push_back(newScope);
}
void SQLContext::popCurrentScope() {
   if (currentScope->parent) {
      std::cerr << "Cannot pop root scope" << std::endl;
      return;
   }
   currentScope = currentScope->parent;
   scopes.pop_back();
}
std::vector<std::pair<size_t, ast::ColumnInfo>> SQLContext::findColumn(const std::string& columnName) const {
   std::vector<std::pair<size_t, ast::ColumnInfo>> columns{};
   std::string foundTable;
   auto current = currentScope;
   while (current) {
      for (auto t : current->columns) {
         auto colsOfTable = t.second;
         foundTable = t.first;
         auto it = std::find_if(colsOfTable.begin(), colsOfTable.end(), [&columnName](std::pair<size_t, ast::ColumnInfo> col) {
            const std::string& colName = col.second.column.getColumnName();
            return colName.length() == columnName.length() &&
               std::equal(colName.begin(), colName.end(), columnName.begin(),
                          [](char a, char b) { return std::tolower(a) == std::tolower(b); });
         });
         if (it != colsOfTable.end()) {
            columns.emplace_back(*it);
         }
      }
      current = current->parent;
   }
   return columns;
}

std::vector<std::pair<size_t, ast::ColumnInfo>> SQLContext::findColumn(const size_t id) const {
   std::vector<std::pair<size_t, ast::ColumnInfo>> columns{};
   std::string foundTable;
   auto current = currentScope;
   while (current) {
      for (auto t : current->columns) {
         auto colsOfTable = t.second;
         foundTable = t.first;
         auto it = std::find_if(colsOfTable.begin(), colsOfTable.end(), [&id](std::pair<size_t, ast::ColumnInfo> col) {
            return col.first == id;
         });
         if (it != colsOfTable.end()) {
            columns.emplace_back(*it);
         }
      }
      current = current->parent;
   }
   return columns;
}

std::vector<std::pair<size_t, ast::ColumnInfo>> SQLContext::findColumn(const std::string& columnName, const std::string& alias) const {
   std::vector<std::pair<size_t, ast::ColumnInfo>> columns{};
   auto current = currentScope;
   while (current) {
      auto table = current->columns.find(alias);
      if (table != current->columns.end()) {
         auto colsOfTable = table->second;
         auto it = std::find_if(colsOfTable.begin(), colsOfTable.end(), [&columnName](std::pair<size_t, ast::ColumnInfo> col) {
            const std::string& colName = col.second.column.getColumnName();
            return colName.length() == columnName.length() &&
               std::equal(colName.begin(), colName.end(), columnName.begin(),
                          [](char a, char b) { return std::tolower(a) == std::tolower(b); });
         });
         if (it != colsOfTable.end()) {
            columns.emplace_back(*it);
         }
      }
      current = current->parent;
   }

   return columns;
}

std::optional<std::pair<size_t, ast::FunctionInfo>> SQLContext::findFunction(const std::string& functionName) const {
   auto current = currentScope;
   while (current) {
      //TODO use hashmap for faster search
      auto function = std::find_if(current->functionsEntry.begin(), current->functionsEntry.end(), [&functionName](const auto& entry) {
         return entry.first == functionName;
      });
      if (function != current->functionsEntry.end()) {
         auto functionInfos = function->second;
         if (functionInfos.empty()) {
            return std::nullopt;
         }
         return functionInfos[0];
      }
      current = current->parent;
   }
   return std::nullopt;
}

std::optional<std::pair<size_t, ast::FunctionInfo>> SQLContext::findFunction(const size_t id) const {
   auto current = currentScope;
   while (current) {
      //TODO use hashmap for faster search
      auto function = std::find_if(current->functionsEntry.begin(), current->functionsEntry.end(), [&id](const auto& entry) {
         return entry.second[0].first == id;
      });
      if (function != current->functionsEntry.end()) {
         auto functionInfos = function->second;
         if (functionInfos.empty()) {
            return std::nullopt;
         }
         return functionInfos[0];
      }
      current = current->parent;
   }
   return std::nullopt;
}

std::vector<std::pair<std::string, catalog::Column>> SQLContext::getColumns() const {
   throw std::runtime_error("Not implemented getColumns");
   /* std::vector<std::pair<std::string, catalog::Column>> columns{};
   for (auto [tableName, table] : currentScope->tables) {
      std::transform(table->getColumns().begin(), table->getColumns().end(), std::back_inserter(columns), [&tableName](catalog::Column column) {
         return std::make_pair(tableName, column);
      });
   }
   return columns;*/
}
std::vector<std::pair<std::string, catalog::Column>> SQLContext::getColumns(std::string& tableName) const {
   throw std::runtime_error("Not implemented getColumns");
   /* auto table = currentScope->tables.find(tableName);
   std::vector<std::pair<std::string, catalog::Column>> columns{};
   std::transform(table->second->getColumns().begin(), table->second->getColumns().end(), std::back_inserter(columns), [&tableName](catalog::Column column) {
      return std::make_pair(tableName, column);
   });
   return columns;*/
}

std::string SQLContext::toString() const {
   return "";
}

} // namespace lingodb::analyzer