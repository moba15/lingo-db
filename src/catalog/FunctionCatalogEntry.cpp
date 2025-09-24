#include "lingodb/catalog/FunctionCatalogEntry.h"

#include "lingodb/utility/Serialization.h"

#include "lingodb/catalog/UDFImplementer.h"
#include "lingodb/catalog/MLIRTypes.h"
#include "lingodb/catalog/TableCatalogEntry.h"
#include "lingodb/compiler/Dialect/DB/IR/RuntimeFunctions.h"

#include <filesystem>
namespace lingodb::catalog {
void FunctionCatalogEntry::serializeEntry(lingodb::utility::Serializer& serializer) const {
   serializer.writeProperty(1, entryType);
   serializer.writeProperty(2, name);
   serializer.writeProperty(3, code);
   serializer.writeProperty(4, returnType);
   serializer.writeProperty(5, argumentTypes);

}
std::shared_ptr<FunctionCatalogEntry> FunctionCatalogEntry::deserialize(lingodb::utility::Deserializer& deserializer) {
   auto entryType = deserializer.readProperty<CatalogEntryType>(1);
   switch (entryType) {
      case CatalogEntryType::C_FUNCTION_ENTRY:
         return CFunctionCatalogEntry::deserialize(deserializer);
      case CatalogEntryType::PLPGSQL_FUNCTION_ENTRY:
         return PLPGSQLFunctionCatalogEntry::deserialize(deserializer);
      default:
            throw std::runtime_error("Should not happen");
   }
}

std::shared_ptr<MLIRUDFImplementor> CFunctionCatalogEntry::getImplementer() {
   return compiler::frontend::createCUDFImplementer(name, code, argumentTypes, returnType);
}

std::shared_ptr<FunctionCatalogEntry> CFunctionCatalogEntry::deserialize(lingodb::utility::Deserializer& deserializer) {
   auto name = deserializer.readProperty<std::string>(2);
   auto code = deserializer.readProperty<std::string>(3);
   auto returnType = deserializer.readProperty<Type>(4);
   auto argumentTypes = deserializer.readProperty<std::vector<Type>>(5);
   return std::make_shared<CFunctionCatalogEntry>(name, code, returnType, argumentTypes);
}


std::shared_ptr<MLIRUDFImplementor> PLPGSQLFunctionCatalogEntry::getImplementer(){
   return compiler::frontend::createPLPGSQLUDFImplementer(name, code, argumentTypes, returnType);
}

std::shared_ptr<FunctionCatalogEntry> PLPGSQLFunctionCatalogEntry::deserialize(lingodb::utility::Deserializer& deserializer){
   auto name = deserializer.readProperty<std::string>(2);
   auto code = deserializer.readProperty<std::string>(3);
   auto returnType = deserializer.readProperty<Type>(4);
   auto argumentTypes = deserializer.readProperty<std::vector<Type>>(5);
   return std::make_shared<PLPGSQLFunctionCatalogEntry>(name, code, returnType, argumentTypes);
}
} // namespace lingodb::catalog