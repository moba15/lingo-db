#include "lingodb/runtime/PythonRuntime.h"


#include "lingodb/runtime/WASM.h"
#include <iostream>
#include <thread>
#include <Python.h>

namespace lingodb::runtime {
/// This method handles all necessary steps before python functions can be called
class PyGuard {
   public:
   PyGuard() : gstate(PyGILState_Ensure()) {}
   ~PyGuard() { PyGILState_Release(gstate); }

   private:
   PyGILState_STATE gstate;
};
inline void throw_python_error(wasm::WASMSession session) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   PythonRuntime::PyObjectPtr ptype = session.createWasmBuffer(sizeof(PyObject));
   PythonRuntime::PyObjectPtr pvalue = session.createWasmBuffer(sizeof(PyObject));
   PythonRuntime::PyObjectPtr ptraceback = session.createWasmBuffer(sizeof(PyObject));
   session.callPyFunc<void>("PyErr_Fetch", ptype, pvalue, ptraceback);
   throw std::runtime_error("msg");
}

PythonRuntime::PyObjectPtr PythonRuntime::createModule(runtime::VarLen32 modname, runtime::VarLen32 source) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   auto modStr = modname.str();
   auto sourceStr = source.str();
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   auto modWasmStr = wasmSession.createWasmStringBuffer(modStr) ;
   auto sourceWasmStr = wasmSession.createWasmStringBuffer(sourceStr);
   PyObjectPtr sys_modules = wasmSession.callPyFunc<PyObjectPtr>("PyImport_GetModuleDict").at(0).of.i32;
   assert(sys_modules);
   // 1) Check if module already exists
   PyObjectPtr mod = wasmSession.callPyFunc<PyObjectPtr>("PyDict_GetItemString",sys_modules, modWasmStr).at(0).of.i32;
   if (mod) {
      wasmSession.callPyFunc<PyObjectPtr>("Py_INCREF",mod);// convert borrowed → owned
      return mod;
   }
   // 2) Create new module
   mod = wasmSession.callPyFunc<PyObjectPtr>("PyModule_New", modWasmStr).at(0).of.i32;
   if (!mod) {
      std::cerr << "Error creating module" << std::endl;
      return 0;
   }

   PyObjectPtr globals = wasmSession.callPyFunc<PyObjectPtr>("PyModule_GetDict", mod).at(0).of.i32;
   assert(globals);
   // Ensure __builtins__
   auto builtinsWasmStr = wasmSession.createWasmStringBuffer("__builtins__");
   if (!wasmSession.callPyFunc<int>("PyDict_GetItemString", globals,builtinsWasmStr ).at(0).of.i32) {
      PyObjectPtr builtins = wasmSession.callPyFunc<PyObjectPtr>("PyEval_GetBuiltins").at(0).of.i32;
      if (wasmSession.callPyFunc<int>("PyDict_SetItemString", globals, builtinsWasmStr, builtins).at(0).of.i32 < 0) {
         std::cerr << "Error " << std::endl;
         return 0;
      }
   }
   auto res = wasmSession.callPyFunc<PyObjectPtr>("PyRun_String", sourceWasmStr, Py_file_input, globals, globals).at(0).of.i32;
   if (!res) {
      throw_python_error(wasmSession);
      assert(false);
   }



   // 3) Insert into sys.modules
   if (wasmSession.callPyFunc<int>("PyDict_SetItemString", sys_modules, modWasmStr, mod).at(0).of.i32 < 0) {
      wasmSession.callPyFunc<void>("Py_DECREF", mod);
      return 0 ;
   }

   return mod;

}
PythonRuntime::PyObjectPtr PythonRuntime::getAttr(PyObjectPtr obj, runtime::VarLen32 attr){
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   auto wasmAttrStr = wasmSession.createWasmStringBuffer(attr.str());
   assert(wasmAttrStr && obj);
   uint32_t pyAttr =  wasmSession.callPyFunc<PyObjectPtr>("PyObject_GetAttrString", obj, wasmAttrStr).at(0).of.i32;
   if (!pyAttr) {
      wasmSession.callPyFunc<void>("PyErr_Print");
   }
   return pyAttr;
}

template <typename... Args>
uint32_t PythonRuntime::callPythonWASMUDF(PyObjectPtr callable, Args&&... args) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   auto result = wasmSession.callPyFunc<PyObjectPtr>("PyObject_CallFunctionObjArgs",callable, args..., 0).at(0).of.i32;
   assert(callable);
   return result;
}

uint32_t PythonRuntime::call0(PyObjectPtr callable) {
   return callPythonWASMUDF(callable);
}

uint32_t PythonRuntime::call1(PyObjectPtr callable, PyObjectPtr arg) {
   return callPythonWASMUDF(callable, arg);
}

uint32_t PythonRuntime::call2(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1) {
   return callPythonWASMUDF(callable, arg, arg1);
}

uint32_t PythonRuntime::call3(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2) {
   return callPythonWASMUDF(callable, arg, arg1, arg2);
}

uint32_t PythonRuntime::call4(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3) {
   return callPythonWASMUDF(callable, arg, arg1, arg2, arg3);
}

uint32_t PythonRuntime::call5(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4) {
   return callPythonWASMUDF(callable, arg, arg1, arg2, arg3, arg4);
}

uint32_t PythonRuntime::call6(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5) {
   return callPythonWASMUDF(callable, arg, arg1, arg2, arg3, arg4, arg5);
}

uint32_t PythonRuntime::call7(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6) {
   return callPythonWASMUDF(callable, arg, arg1, arg2, arg3, arg4, arg5, arg6);
}

uint32_t PythonRuntime::call8(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7) {
   return callPythonWASMUDF(callable, arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

uint32_t PythonRuntime::call9(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7, PyObjectPtr arg8) {
   return callPythonWASMUDF(callable, arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

uint32_t PythonRuntime::call10(PyObjectPtr callable, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7, PyObjectPtr arg8, PyObjectPtr arg9) {
   return callPythonWASMUDF(callable, arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

uint64_t PythonRuntime::toInt64(PyObjectPtr pyObj) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   return wasmSession.callPyFunc<uint64_t>("PyLong_AsLongLong", pyObj).at(0).of.i64;
}

PythonRuntime::PyObjectPtr PythonRuntime::fromInt64(int64_t obj) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   return wasmSession.callPyFunc<PyObjectPtr>("PyLong_FromLongLong", obj).at(0).of.i32;
}
bool PythonRuntime::toBool(PyObjectPtr obj){
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   return wasmSession.callPyFunc<int>("PyObject_IsTrue", obj).at(0).of.i32; //todo
}
PythonRuntime::PyObjectPtr PythonRuntime::fromBool(bool value){
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   return wasmSession.callPyFunc<PyObjectPtr>("PyBool_FromLong", value ? 1: 0).at(0).of.i32;
}
runtime::VarLen32 PythonRuntime::toVarLen32(PyObjectPtr obj){

}
PythonRuntime::PyObjectPtr PythonRuntime::fromVarLen32(runtime::VarLen32 value){
}
double PythonRuntime::toDouble(PyObjectPtr obj){
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   return wasmSession.callPyFunc<double>("PyFloat_AsDouble", obj).at(0).of.f64;
}
PythonRuntime::PyObjectPtr PythonRuntime::fromDouble(double value){
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   return wasmSession.callPyFunc<PyObjectPtr>("PyFloat_FromDouble", value).at(0).of.i32;
}
void PythonRuntime::decref(PyObjectPtr obj){
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   wasmSession.callPyFunc<void>("Py_DecRef", obj);
}
void PythonRuntime::incref(PyObjectPtr obj){
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   wasmSession.callPyFunc<void>("Py_IncRef", obj);
}
PythonRuntime::PyObjectPtr PythonRuntime::import(runtime::VarLen32 val){
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   auto wasmStr = wasmSession.createWasmStringBuffer(val.str());
   return wasmSession.callPyFunc<PyObjectPtr>("PyImport_ImportModule", wasmStr).at(0).of.i32;
}
/*PythonRuntime::PyObjectPtr PythonRuntime::int64ToPythonLong(int64_t value) {
   assert(wasm_runtime_thread_env_inited());
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   assert(wasmSession.callPyFunc<bool>("Py_IsInitialized").at(0).of.i32);
   return wasmSession.callPyFunc<PyObjectPtr>("PyLong_FromLongLong", value).at(0).of.i32;
}
PythonRuntime::PyObjectPtr PythonRuntime::int32ToPythonInt(int32_t value) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   assert(wasmSession.callPyFunc<bool>("Py_IsInitialized").at(0).of.i32);
   return wasmSession.callPyFunc<PyObjectPtr>("PyLong_FromLong", value).at(0).of.i32;
}
PythonRuntime::PyObjectPtr PythonRuntime::floatToPythonFloat(float value) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   assert(wasmSession.callPyFunc<bool>("Py_IsInitialized").at(0).of.i32);
   return wasmSession.callPyFunc<PyObjectPtr>("PyFloat_FromDouble",  value).at(0).of.i32;
}
PythonRuntime::PyObjectPtr PythonRuntime::stringToPythonString(VarLen32 value) {
   throw std::runtime_error("Not impl");
   PyGuard pyGuard{};
   if (!Py_IsInitialized()) {
      throw std::runtime_error("Python not initialized");
   }
   PyObject* pyValue = PyUnicode_FromString(value.str().c_str());
   if (!pyValue) {
      throw std::runtime_error("Could not convert double to python float");
   }
   return reinterpret_cast<uint64_t>(pyValue);
}

int32_t PythonRuntime::pythonIntToInt32(PyObjectPtr pyObj) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   assert(wasmSession.callPyFunc<bool>("Py_IsInitialized").at(0).of.i32);
   return wasmSession.callPyFunc<int32_t>("PyLong_AsLong", pyObj).at(0).of.i32;
}
float PythonRuntime::pythonFloatToFloat(uint32_t pyObj) {
#ifdef ASAN_ACTIVE
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
#endif
   wasm::WASMSession wasmSession = getCurrentExecutionContext()->getWasmSession();
   assert(wasmSession.callPyFunc<bool>("Py_IsInitialized").at(0).of.i32);
   auto result = wasmSession.callPyFunc<double>("PyFloat_AsDouble", pyObj).at(0);
   switch (result.kind) {
      case WASM_F64:
         return result.of.f32;
         case WASM_F32:
         return result.of.f32;
      default: throw (std::runtime_error("Python not initialized"));
   }

}
VarLen32 PythonRuntime::pythonStringToString(uint64_t pyObj) {
   throw std::runtime_error("Not impl");
   PyGuard pyGuard{};
   if (!Py_IsInitialized()) {
      throw std::runtime_error("Python not initialized");
   }
   PyObject* pValue = reinterpret_cast<PyObject*>(pyObj);
   if (PyUnicode_Check(pValue)) {
      PyObject* utf8 = PyUnicode_AsUTF8String(pValue);
      if (!utf8) {
         Py_DECREF(pValue);
         throw std::runtime_error("Could not convert python string to utf8");
      }
      char* str = PyBytes_AsString(utf8);
      VarLen32 result = VarLen32::fromString(std::string(str));
      Py_DECREF(utf8);
      Py_DECREF(pValue);
      return result;
   } else {
      Py_DECREF(pValue);
      throw std::runtime_error("Provided object is not a python string");
   }
}*/
} // namespace lingodb::runtime
