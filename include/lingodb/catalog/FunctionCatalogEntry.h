#pragma once
#include "Catalog.h"
#include "Column.h"
#include "UDFImplementer.h"

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
   static constexpr std::array<CatalogEntryType, 1> entryTypes = {CatalogEntryType::C_FUNCTION_ENTRY};
   FunctionCatalogEntry( std::string name, std::string code, Type returnType, std::vector<Type> argumentTypes)
      : CatalogEntry(CatalogEntryType::C_FUNCTION_ENTRY), name(std::move(name)), code(std::move(code)), returnType(std::move(returnType)), argumentTypes(std::move(argumentTypes)) {}
   std::string getName() override { return name; }
   [[nodiscard]] std::string getCode() const { return code; }
   [[nodiscard]] Type getReturnType() const { return returnType; }
   [[nodiscard]] std::vector<Type> getArgumentTypes() const { return argumentTypes; }
   void serializeEntry(lingodb::utility::Serializer& serializer) const override;

   virtual std::shared_ptr<MLIRUDFImplementor> getImplementer() = 0;

   static std::shared_ptr<FunctionCatalogEntry> deserialize(lingodb::utility::Deserializer& deserializer);
};

class CFunctionCatalogEntry : public  FunctionCatalogEntry {
   public:
   CFunctionCatalogEntry( std::string name, std::string code, Type returnType, std::vector<Type> argumentTypes)
      : FunctionCatalogEntry(name, code, returnType, argumentTypes) {}
   std::shared_ptr<MLIRUDFImplementor> getImplementer() override;

   static std::shared_ptr<FunctionCatalogEntry> deserialize(lingodb::utility::Deserializer& deserializer);
};
} // namespace lingodb::catalog