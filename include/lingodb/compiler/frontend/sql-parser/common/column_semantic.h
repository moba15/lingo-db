#pragma once
#include "lingodb/catalog/Column.h"

#include <string>
#include <vector>
namespace lingodb::ast {
struct ColumnInfo {
   std::string scope;
   catalog::Column column;
};
struct TargetInfo {
   std::vector<std::pair<std::string, ColumnInfo>> namedResults;
   void map(std::string name, const ColumnInfo cInfo) {
      namedResults.push_back({name, cInfo});
   }
};
}