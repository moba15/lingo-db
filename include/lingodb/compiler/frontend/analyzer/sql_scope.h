#pragma once
#include "lingodb/catalog/Defs.h"
#include "lingodb/catalog/TableCatalogEntry.h"

#include <map>
#include <memory>
namespace lingodb::analyzer {
class SQLScope {
   public:
   std::map<std::string, std::shared_ptr<catalog::TableCatalogEntry>> tables;
   std::shared_ptr<SQLScope> parent;
};
}