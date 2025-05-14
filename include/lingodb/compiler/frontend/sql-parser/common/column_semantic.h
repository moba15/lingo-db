#pragma once
#include "lingodb/catalog/Column.h"

#include <string>
#include <vector>
namespace lingodb::ast {
enum class NamedResultType : uint8_t {
   Column = 1,
   Function = 2
};
struct NamedResult {
   NamedResultType type;
   std::string scope;
   NamedResult(NamedResultType type, std::string scope) : type(type), scope(scope) {}
};
struct FunctionInfo : public NamedResult {
   std::string name;
   FunctionInfo(std::string scope, std::string name) : NamedResult(NamedResultType::Function, scope), name(name) {}
};
struct ColumnInfo : public  NamedResult {
   catalog::Column column;
   ColumnInfo(std::string scope, catalog::Column column) : NamedResult(NamedResultType::Column, scope), column(column) {}
};
struct TargetInfo {
   std::vector<std::pair<std::string, std::shared_ptr<NamedResult>>> namedResults;
   void map(std::string name, std::shared_ptr<NamedResult> cInfo) {
      namedResults.push_back({name, std::move(cInfo)});
   }
};
} // namespace lingodb::ast