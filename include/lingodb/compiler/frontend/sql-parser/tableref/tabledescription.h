#pragma once
#include <string>
namespace lingodb::ast {
class TableDescription {
   public:
   TableDescription(std::string database, std::string schema, std::string table) : database(database), schema(schema), table(table) {};

   std::string database;
   std::string schema;
   std::string table;
   bool readonly = true;
   //TODO column definitions
};
}