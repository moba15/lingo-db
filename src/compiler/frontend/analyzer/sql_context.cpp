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
   for (auto t : currentScope->tables) {
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
   return std::make_pair(foundTable, columns);
}

std::pair<std::string, std::vector<catalog::Column>> SQLContext::findColumn(const std::string& columnName, const std::string& alias) const {
   std::vector<catalog::Column> columns{};
   auto table = currentScope->tables.find(alias);
   if (table == currentScope->tables.end()) {
      return std::make_pair("", columns);
   }

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

   return std::make_pair(alias, columns);
}
} // namespace lingodb::analyzer