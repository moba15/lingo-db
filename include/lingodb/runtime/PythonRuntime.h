#ifndef LINGODB_UDFRUNTIME_H
#define LINGODB_UDFRUNTIME_H

#include "lingodb/runtime/helpers.h"


namespace lingodb::runtime {
struct PythonRuntime {
   using PyObjectPtr = uint32_t;

   static PyObjectPtr createModule(runtime::VarLen32 modname, runtime::VarLen32 source);
   static PyObjectPtr getAttr(PyObjectPtr obj, runtime::VarLen32 attr);

   template <typename... Args>
   static uint32_t callPythonWASMUDF(PyObjectPtr callable, Args&&... args);

   static uint32_t call0(PyObjectPtr callable);
   static uint32_t call1(PyObjectPtr callable, PyObjectPtr arg);
   static uint32_t call2(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1);
   static uint32_t call3(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2);
   static uint32_t call4(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3);
   static uint32_t call5(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4);
   static uint32_t call6(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5);
   static uint32_t call7(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6);
   static uint32_t call8(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7);
   static uint32_t call9(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7, PyObjectPtr arg8);
   static uint32_t call10(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7, PyObjectPtr arg8, PyObjectPtr arg9);
   static uint64_t toInt64(PyObjectPtr pyObj);
   static PyObjectPtr fromInt64(int64_t value);
   static bool toBool(PyObjectPtr obj);
   static PyObjectPtr fromBool(bool value);
   static runtime::VarLen32 toVarLen32(PyObjectPtr obj);
   static PyObjectPtr fromVarLen32(runtime::VarLen32 value);
   static double toDouble(PyObjectPtr obj);
   static PyObjectPtr fromDouble(double value);
   static void decref(PyObjectPtr obj);
   static void incref(PyObjectPtr obj);
   static PyObjectPtr import(runtime::VarLen32 val);
};

} // namespace lingodb::runtime
#endif //LINGODB_UDFRUNTIME_H
