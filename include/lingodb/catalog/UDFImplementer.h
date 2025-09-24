#pragma once
#include "lingodb/catalog/MLIRTypes.h"

#include <lingodb/catalog/Types.h>
namespace lingodb::catalog {
class MLIRUDFImplementor {
   public:
   virtual mlir::Value callFunction(mlir::ModuleOp& moduleOp, mlir::OpBuilder& builder, mlir::Location loc, mlir::ValueRange args) = 0;
   virtual ~MLIRUDFImplementor() = default;
};
void visitUDFFunctions(const std::function<void(std::string, void*)>& fn);
} // namespace lingodb::catalog

namespace lingodb::compiler::frontend {
std::shared_ptr<catalog::MLIRUDFImplementor> createCUDFImplementer(
   std::string funcName, std::string cCode, std::vector<lingodb::catalog::Type> argumentTypes, lingodb::catalog::Type returnType);

std::shared_ptr<catalog::MLIRUDFImplementor> createPLPGSQLUDFImplementer(
   std::string funcName, std::string cCode, std::vector<lingodb::catalog::Type> argumentTypes, lingodb::catalog::Type returnType);

} // namespace lingodb::compiler::frontend