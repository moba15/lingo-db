#ifndef LINGODB_UDFRUNTIME_H
#define LINGODB_UDFRUNTIME_H
#include "lingodb/runtime/helpers.h"

namespace lingodb::runtime {
struct UDFRuntime {
   static double callPythonUDF(lingodb::runtime::VarLen32, std::vector<lingodb::runtime::VarLen32>);
};

} // namespace lingodb::runtime
#endif //LINGODB_UDFRUNTIME_H
