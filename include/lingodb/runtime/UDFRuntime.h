#ifndef LINGODB_UDFRUNTIME_H
#define LINGODB_UDFRUNTIME_H
#include "lingodb/runtime/helpers.h"

namespace lingodb::runtime {
struct UDFRuntime {
   static int callPythonUDF(VarLen32* args);
};

} // namespace lingodb::runtime
#endif //LINGODB_UDFRUNTIME_H
