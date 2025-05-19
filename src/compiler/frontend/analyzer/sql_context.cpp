#include "lingodb/compiler/frontend/analyzer/sql_context.h"

#include <iostream>
namespace lingodb::analyzer {
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
std::pair<std::string, std::vector<catalog::Column>> SQLContext::findColumn(const std::string& columnName) const {
   std::vector<catalog::Column> columns{};
   std::string foundTable;
   auto current = currentScope;
   while (current) {
      for (auto t : current->tables) {
         auto colsOfTable = t.second->getColumns();
         foundTable = t.first;
         auto it = std::find_if(colsOfTable.begin(), colsOfTable.end(), [&columnName](const catalog::Column& col) {
            const std::string& colName = col.getColumnName();
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
   return std::make_pair(foundTable, columns);
}

std::pair<std::string, std::vector<catalog::Column>> SQLContext::findColumn(const std::string& columnName, const std::string& alias) const {
   std::vector<catalog::Column> columns{};
   auto current = currentScope;
   while (current) {
      auto table = current->tables.find(alias);
      if (table != current->tables.end()) {
         auto colsOfTable = table->second->getColumns();
         auto it = std::find_if(colsOfTable.begin(), colsOfTable.end(), [&columnName](const catalog::Column& col) {
            const std::string& colName = col.getColumnName();
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

   return std::make_pair(alias, columns);
}

std::pair<std::string, std::shared_ptr<ast::FunctionInfo>> SQLContext::findFunction(const std::string& functionName) const {
   auto current = currentScope;
   while (current) {
      //TODO use hashmap for faster search
      auto function = std::find_if(current->functionsEntry.begin(), current->functionsEntry.end(), [&functionName](const auto& entry) {
         return entry.first == functionName;
      });
      if (function != current->functionsEntry.end()) {
         return std::make_pair(function->first, function->second);
      }
      current = current->parent;
   }
   return std::make_pair("", nullptr);
}

std::vector<std::pair<std::string, catalog::Column>> SQLContext::getColumns() const {
   std::vector<std::pair<std::string, catalog::Column>> columns{};
   for (auto [tableName, table] : currentScope->tables) {
      std::transform(table->getColumns().begin(), table->getColumns().end(), std::back_inserter(columns), [&tableName](catalog::Column column) {
         return std::make_pair(tableName, column);
      });
   }
   return columns;
}
std::vector<std::pair<std::string, catalog::Column>> SQLContext::getColumns(std::string& tableName) const {
   auto table = currentScope->tables.find(tableName);
   std::vector<std::pair<std::string, catalog::Column>> columns{};
   std::transform(table->second->getColumns().begin(), table->second->getColumns().end(), std::back_inserter(columns), [&tableName](catalog::Column column) {
      return std::make_pair(tableName, column);
   });
   return columns;
}

std::string SQLContext::toString() const {
   std::ostringstream oss{};
   oss << "SQLContext:\n";
   auto current = currentScope;
   for (auto scope : scopes) {
      for (const auto& [tableName, table] : scope->tables) {
         oss << "  Tabelle: " << tableName << "\n";
         for (const auto& column : table->getColumns()) {
            oss << "    - Spalte: " << std::setw(20) << column.getColumnName()
                << " | Typ: " << column.getLogicalType().toString() << "\n";
         }
      }
   }

   return oss.str();
}

} // namespace lingodb::analyzer