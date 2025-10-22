#ifndef LINGODB_UDFRUNTIME_H
#define LINGODB_UDFRUNTIME_H
#include "lingodb/runtime/helpers.h"

namespace lingodb::runtime {
struct UDFRuntime {
   static uint64_t callPythonUDF(int64_t arg1);
   static uint64_t callPythonUDF4(VarLen32 fnName, int64_t arg, int64_t arg1, int64_t arg2, int64_t arg3);
   static uint64_t int64ToPythonLong(int64_t value);
   static uint64_t int32ToPythonInt(int32_t value);
   static uint64_t floatToPythonFloat(float value);
   static uint64_t doubleToPythonDouble(double value);

   static uint64_t pythonLongToInt64(uint64_t pyObj);
   static int32_t pythonIntToInt32(uint64_t pyObj);
   static double pythonDoubleToDouble(uint64_t pyObj);

};

} // namespace lingodb::runtime
#endif //LINGODB_UDFRUNTIME_H
