#pragma once
#include "lingodb/catalog/Defs.h"
#include "lingodb/catalog/TableCatalogEntry.h"
#include "lingodb/compiler/frontend/sql-parser/common/column_semantic.h"

#include <map>
#include <memory>
namespace lingodb::analyzer {

class SQLScope {
   public:
   std::map<std::string, std::pair<size_t, ast::ColumnInfo>> tables;
   std::map<std::string, std::vector<std::pair<size_t, ast::ColumnInfo>>> columns;
   std::map<std::string, std::vector<std::pair<size_t, ast::FunctionInfo>>> functionsEntry;
   //Stores the columns that should be produced at the end
   ast::TargetInfo targetInfo;
   std::shared_ptr<SQLScope> parent;
   size_t id = 0;

   ast::BoundColumnEntry addTableCatalogEntry(std::string name, std::shared_ptr<catalog::TableCatalogEntry> table) {
      std::vector<std::pair<size_t, ast::ColumnInfo>> columInfos;
      for (auto c : table->getColumns()) {
         columInfos.emplace_back(std::make_pair(id, ast::ColumnInfo(name, c)));
         id++;
      }
      columns.emplace(name, std::move(columInfos));

      return ast::BoundColumnEntry(id);
   };

   std::shared_ptr<ast::BoundColumnEntry> addFunctionEntry(std::string name, ast::FunctionInfo function) {
      functionsEntry.emplace(name, std::vector{std::make_pair(id, function)});
      id++;
      return std::make_shared<ast::BoundColumnEntry>(id - 1);
   }
};
} // namespace lingodb::analyzer