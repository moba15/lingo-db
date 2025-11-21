#ifndef LINGODB_UDFRUNTIME_H
#define LINGODB_UDFRUNTIME_H

#include "lingodb/runtime/helpers.h"

namespace lingodb::runtime {
struct PythonUDFRuntime {
   using PyObjectPtr = uint32_t;
   template <unsigned SIZE>
   static uint64_t callPythonUDF(std::string fnName, std::array<uint64_t, SIZE> args);
   template <unsigned SIZE>
   static uint32_t callPythonWASMUDF(std::string fnName, std::array<PyObjectPtr, SIZE> args);

   static uint32_t callPythonUDF0(VarLen32 fnName);
   static uint32_t callPythonUDF1(VarLen32 fnName, PyObjectPtr arg);
   static uint32_t callPythonUDF2(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1);
   static uint32_t callPythonUDF3(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2);
   static uint32_t callPythonUDF4(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3);
   static uint32_t callPythonUDF5(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4);
   static uint32_t callPythonUDF6(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5);
   static uint32_t callPythonUDF7(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6);
   static uint32_t callPythonUDF8(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7);
   static uint32_t callPythonUDF9(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7, PyObjectPtr arg8);
   static uint32_t callPythonUDF10(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7, PyObjectPtr arg8, PyObjectPtr arg9);

   static PyObjectPtr int64ToPythonLong(int64_t value);
   static PyObjectPtr int32ToPythonInt(int32_t value);
   static PyObjectPtr floatToPythonFloat(float value);
   static PyObjectPtr stringToPythonString(VarLen32 value);

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
   static float pythonFloatToFloat(uint32_t pyObj);
   /***
    * IMPORTANT: This method call decref on the python object after conversion
    */
   static VarLen32 pythonStringToString(uint64_t pyObj);
};

} // namespace lingodb::runtime
#endif //LINGODB_UDFRUNTIME_H
