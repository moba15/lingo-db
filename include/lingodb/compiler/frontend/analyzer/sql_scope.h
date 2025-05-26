#pragma once
#include "lingodb/catalog/Defs.h"
#include "lingodb/catalog/TableCatalogEntry.h"
#include "lingodb/compiler/frontend/sql-parser/common/column_semantic.h"

#include <map>
#include <memory>
namespace lingodb::analyzer {
class SQLScope {
   public:
   std::map<std::string, std::shared_ptr<catalog::TableCatalogEntry>> tables;
   std::map<std::string, std::shared_ptr<ast::FunctionInfo>> functionsEntry;
   //Stores the columns that should be produced at the end
   ast::TargetInfo targetInfo;
   std::shared_ptr<SQLScope> parent;
};
}