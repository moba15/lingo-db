#pragma once

#include "lingodb/compiler/frontend/ast/create_node.h"


namespace lingodb::ast {

class BoundColumnElement : public TableElement {
   public:
   BoundColumnElement(std::string name, catalog::NullableType datatype)
         : TableElement(TableElementType::COLUMN), name(std::move(name)), datatype(datatype) {}


   std::string name;
   catalog::NullableType datatype;

};



}