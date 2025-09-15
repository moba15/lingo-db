#pragma once
#include "lingodb/catalog/MLIRTypes.h"

#include <lingodb/catalog/Types.h>

namespace lingodb::compiler::frontend {
std::shared_ptr<lingodb::catalog::MLIRUDFImplementor> createCUDFImplementer(
   std::string funcName, std::string cCode, std::vector<lingodb::catalog::Type> argumentTypes, lingodb::catalog::Type returnType);
}