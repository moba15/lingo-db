#pragma once
#include "lingodb/catalog/Column.h"
#include "lingodb/compiler/Dialect/TupleStream/ColumnManager.h"

#include <string>
#include <vector>
namespace lingodb::ast {
class BoundFunctionExpression;

enum class NamedResultType : uint8_t {
   Column = 1,
   Function = 2,
   EXPRESSION = 3,
};
struct NamedResult {
   NamedResultType type;
   std::string scope;
   catalog::NullableType resultType;
   std::string name;
   std::string displayName{"no display name"};
   std::optional<mlir::Type> mlirType;
   NamedResult(NamedResultType type, std::string scope, catalog::NullableType resultType, std::string name) : type(type), scope(scope), resultType(resultType), name(name) {}

    virtual  compiler::dialect::tuples::ColumnRefAttr createRef(compiler::dialect::tuples::ColumnManager& attrManager) {
      return attrManager.createRef(this->scope, name);
   };

   virtual  compiler::dialect::tuples::ColumnDefAttr createDef(compiler::dialect::tuples::ColumnManager& attrManager) {
      return attrManager.createDef(this->scope, name);
   };
};
struct FunctionInfo : public NamedResult {




   FunctionInfo(std::string scope, std::string name, catalog::NullableType resultType) : NamedResult(NamedResultType::Function, scope, resultType, name) {}


};
struct ColumnInfo : public NamedResult {
   catalog::Column column;


   ColumnInfo(std::string scope, catalog::Column column) : NamedResult(NamedResultType::Column, scope,catalog::NullableType(column.getLogicalType(), column.getIsNullable()), column.getColumnName()), column(column) {
      displayName = column.getColumnName();
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
   std::vector<std::shared_ptr<NamedResult>> targetColumns;
   void add(std::shared_ptr<NamedResult> entry) {
      targetColumns.push_back(std::move(entry));
   }

   //std::vector<std::pair<std::string, std::shared_ptr<NamedResult>>> namedResults;
   /*void map(std::string name, std::shared_ptr<NamedResult> cInfo) {
      namedResults.push_back({name, std::move(cInfo)});
   }*/
};
} // namespace lingodb::ast