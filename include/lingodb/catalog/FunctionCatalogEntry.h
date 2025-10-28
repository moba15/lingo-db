#ifndef LINGODB_CATALOG_FUNCTIONCATALOGENTRY_H
#define LINGODB_CATALOG_FUNCTIONCATALOGENTRY_H

#include "Catalog.h"
#include "Column.h"

#include <mlir/IR/BuiltinOps.h.inc>
#include <vector>
namespace lingodb::catalog {
class FunctionCatalogEntry : public CatalogEntry {
   protected:
   std::string name;
   std::string code;
   Type returnType;
   std::vector<Type> argumentTypes;

   public:
   static constexpr std::array<CatalogEntryType, 2> entryTypes = {CatalogEntryType::C_FUNCTION_ENTRY, CatalogEntryType::PYTHON_FUNCTION_ENTRY};
   struct UDFHandle {
      void* handle;
      void* addrPtr;
   };
   static std::unordered_map<std::string, UDFHandle>& getUdfFunctions() {
      static std::unordered_map<std::string, UDFHandle> udfFunctions;
      return udfFunctions;
   }
   FunctionCatalogEntry(CatalogEntryType type, std::string name, std::string code, Type returnType, std::vector<Type> argumentTypes)
      : CatalogEntry(type), name(std::move(name)), code(std::move(code)), returnType(std::move(returnType)), argumentTypes(std::move(argumentTypes)) {}
   std::string getName() override { return name; }
   [[nodiscard]] std::string getCode() const { return code; }
   [[nodiscard]] Type getReturnType() const { return returnType; }
   [[nodiscard]] std::vector<Type> getArgumentTypes() const { return argumentTypes; }
   void serializeEntry(lingodb::utility::Serializer& serializer) const override;

   static std::shared_ptr<FunctionCatalogEntry> deserialize(lingodb::utility::Deserializer& deserializer);
};

class CFunctionCatalogEntry : public FunctionCatalogEntry {
   public:
   CFunctionCatalogEntry(std::string name, std::string code, Type returnType, std::vector<Type> argumentTypes)
      : FunctionCatalogEntry(CatalogEntryType::C_FUNCTION_ENTRY, name, code, returnType, argumentTypes) {}

   static std::shared_ptr<FunctionCatalogEntry> deserialize(lingodb::utility::Deserializer& deserializer);
};

class PythonFunctionCatalogEntry : public FunctionCatalogEntry {
   public:
   PythonFunctionCatalogEntry(std::string name, std::string code, Type returnType, std::vector<Type> argumentTypes)
      : FunctionCatalogEntry(CatalogEntryType::PYTHON_FUNCTION_ENTRY, name, code, returnType, argumentTypes) {}

   static std::shared_ptr<FunctionCatalogEntry> deserialize(lingodb::utility::Deserializer& deserializer);
};

void visitUDFFunctions(const std::function<void(std::string, void*)>& fn);
} // namespace lingodb::catalog
#endif
