#ifndef LINGODB_UDFRUNTIME_H
#define LINGODB_UDFRUNTIME_H
#include "lingodb/runtime/helpers.h"

namespace lingodb::runtime {
struct PythonUDFRuntime {
   template<unsigned SIZE>
   static uint64_t callPythonUDF(std::string fnName, std::array<uint64_t, SIZE> args);

   static uint64_t callPythonUDF1(VarLen32 fnName, uint64_t arg);
   static uint64_t callPythonUDF2(VarLen32 fnName, uint64_t arg, uint64_t arg1);
   static uint64_t callPythonUDF3(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2);
   static uint64_t callPythonUDF4(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3);
   static uint64_t callPythonUDF5(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
   static uint64_t callPythonUDF6(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
   static uint64_t callPythonUDF7(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);
   static uint64_t callPythonUDF8(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7);
   static uint64_t callPythonUDF9(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7, uint64_t arg8);
   static uint64_t callPythonUDF10(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7, uint64_t arg8, uint64_t arg9);

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
