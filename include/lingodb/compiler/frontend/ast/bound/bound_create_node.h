#pragma once

#include "lingodb/compiler/frontend/ast/create_node.h"

namespace lingodb::ast {

class BoundColumnElement : public TableElement {
   public:
   BoundColumnElement(std::string name, NullableType datatype, bool primary)
      : TableElement(TableElementType::COLUMN), name(std::move(name)), datatype(datatype), primary(primary) {}

   std::string name;
   NullableType datatype;

   bool unique;
   bool primary;
};

class BoundCreateFunctionInfo : public CreateInfo {
   public:
   BoundCreateFunctionInfo(std::string functionName, bool replace, NullableType returnType)
      : CreateInfo(CatalogType::TABLE_FUNCTION_ENTRY, std::move(""), std::move(""), false), functionName(functionName), replace(replace), returnType(returnType) {}

   std::string functionName;
   bool replace;
   std::string aliasOf;

   std::string filePath;
   std::string language;
   std::string code;
   NullableType returnType;
   std::vector<catalog::Type> argumentTypes;
   std::vector<std::string> argumentNames;

   std::vector<std::pair<std::string, std::string>> options;


};

}