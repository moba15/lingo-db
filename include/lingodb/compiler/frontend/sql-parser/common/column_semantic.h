#pragma once
#include "lingodb/catalog/Column.h"
#include "lingodb/compiler/Dialect/TupleStream/ColumnManager.h"

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

   virtual compiler::dialect::tuples::ColumnRefAttr createRef(compiler::dialect::tuples::ColumnManager& attrManager) = 0;
};
struct FunctionInfo : public NamedResult {
   std::string name;
   catalog::NullableType resultType;

   FunctionInfo(std::string scope, std::string name, catalog::NullableType resultType) : NamedResult(NamedResultType::Function, scope), name(name), resultType(resultType) {}

   compiler::dialect::tuples::ColumnRefAttr createRef(compiler::dialect::tuples::ColumnManager& attrManager) override {
      return attrManager.createRef(this->scope, name);
   }
};
struct ColumnInfo : public NamedResult {
   catalog::Column column;
   ColumnInfo(std::string scope, catalog::Column column) : NamedResult(NamedResultType::Column, scope), column(column) {}
   compiler::dialect::tuples::ColumnRefAttr createRef(compiler::dialect::tuples::ColumnManager& attrManager) override {
      return attrManager.createRef(this->scope, column.getColumnName());
   }
};
class BoundColumnEntry {
   public:
   size_t index;
   std::string displayName{};

   static std::shared_ptr<BoundColumnEntry> create() {
      static size_t currentId = 0;
      return std::make_shared<BoundColumnEntry>(currentId++);
   }
};
struct TargetInfo {
   public:
   std::vector<std::shared_ptr<BoundColumnEntry>> targetColumns;
   void add(std::shared_ptr<BoundColumnEntry> entry) {
      targetColumns.push_back(std::move(entry));
   }

   //std::vector<std::pair<std::string, std::shared_ptr<NamedResult>>> namedResults;
   /*void map(std::string name, std::shared_ptr<NamedResult> cInfo) {
      namedResults.push_back({name, std::move(cInfo)});
   }*/
};
} // namespace lingodb::ast