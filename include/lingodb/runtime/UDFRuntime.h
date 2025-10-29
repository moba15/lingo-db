#ifndef LINGODB_UDFRUNTIME_H
#define LINGODB_UDFRUNTIME_H
#include "lingodb/runtime/helpers.h"

namespace lingodb::runtime {
struct UDFRuntime {
   static bool isPythonInitialized;
   static uint64_t callPythonUDF(std::string fnName, std::vector<uint64_t> args);
   static uint64_t callPythonUDF2(VarLen32 fnName, uint64_t arg, uint64_t arg1);
   static uint64_t callPythonUDF4(VarLen32 fnName, int64_t arg, int64_t arg1, int64_t arg2, int64_t arg3);
   static uint64_t int64ToPythonLong(int64_t value);
   static uint64_t int32ToPythonInt(int32_t value);
   static uint64_t floatToPythonFloat(float value);
   static uint64_t doubleToPythonDouble(double value);

   /***
    * IMPORTANT: This method call decref on the python object after conversion
    */
   static uint64_t pythonLongToInt64(uint64_t pyObj);
   /***
    * IMPORTANT: This method call decref on the python object after conversion
    */
   static int32_t pythonIntToInt32(uint64_t pyObj);
   /***
    * IMPORTANT: This method call decref on the python object after conversion
    */
   static double pythonDoubleToDouble(uint64_t pyObj);
};

} // namespace lingodb::runtime
#endif //LINGODB_UDFRUNTIME_H
