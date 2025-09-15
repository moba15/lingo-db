#include "lingodb/catalog/FunctionCatalogEntry.h"

#include "lingodb/execution/Execution.h"
#include "lingodb/utility/Serialization.h"
#include "lingodb/runtime/StringRuntime.h"
#include "lingodb/catalog/Defs.h"
#include "lingodb/catalog/FunctionCatalogEntry.h"

#include "lingodb/catalog/CUDF.h"
#include "lingodb/catalog/MLIRTypes.h"
#include "lingodb/catalog/TableCatalogEntry.h"
#include "lingodb/compiler/Dialect/DB/IR/RuntimeFunctions.h"
#include "lingodb/compiler/Dialect/RelAlg/Transforms/queryopt/QueryGraph.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorDialect.h"
#include "lingodb/compiler/Dialect/SubOperator/SubOperatorOps.h"
#include "lingodb/compiler/Dialect/TupleStream/TupleStreamOps.h"
#include "lingodb/compiler/Dialect/util/UtilDialect.h"
#include "lingodb/compiler/frontend/ast/bound/bound_extend_node.h"
#include "lingodb/compiler/frontend/ast/bound/bound_insert_node.h"
#include "lingodb/compiler/frontend/ast/bound/bound_query_node.h"
#include "lingodb/compiler/frontend/ast/bound/bound_tableref.h"
#include "lingodb/compiler/frontend/ast/copy_node.h"
#include "lingodb/compiler/frontend/ast/create_node.h"
#include "lingodb/compiler/frontend/ast/insert_node.h"
#include "lingodb/compiler/frontend/ast/set_node.h"
#include "lingodb/execution/Execution.h"
#include "lingodb/runtime/StringRuntime.h"

#include <filesystem>
#include <dlfcn.h>
#include <mlir/Dialect/Arith/IR/Arith.h>
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
} // namespace lingodb::catalog