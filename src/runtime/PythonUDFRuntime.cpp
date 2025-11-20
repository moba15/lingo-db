#include "lingodb/runtime/PythonUDFRuntime.h"

#include "lingodb/runtime/WASM.h"
#include <iostream>
#include <thread>
#include <Python.h>
namespace lingodb::runtime {
/// This method handles all necessary steps before python functions can be called
void inline initPythonWASMCall() {
#ifdef ASAN_ACTIVE

#endif
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
}
class PyGuard {
   public:
   PyGuard() : gstate(PyGILState_Ensure()) {}
   ~PyGuard() { PyGILState_Release(gstate); }

   private:
   PyGILState_STATE gstate;
};

template <unsigned SIZE>
uint32_t PythonUDFRuntime::callPythonWASMUDF(std::string fnName, std::array<PyObjectPtr, SIZE> args) {
   initPythonWASMCall();
   auto wasmSession = lingodb::wasm::WASM::localWasmSessions[scheduler::currentWorkerId()];
   assert(wasmSession);
   if (!wasmSession->callPyFunc2<bool>("Py_IsInitialized")) {
      throw std::runtime_error{"Py_IsInitialized"};
   };
   auto pArgs = wasmSession->callPyFunc<PyObjectPtr>("PyTuple_New", args.size()).at(0).of.i32;
   if (!pArgs) {
      throw std::runtime_error{"Could not create python tuple"};
   }
   for (size_t i = 0; i < SIZE; i++) {
      if (wasmSession->callPyFunc2<int>("PyTuple_SetItem", pArgs, i, args[i]) != 0) {
         throw std::runtime_error{"Could not add argument to python tuple"};
      }
   }
   //TODO change to use one buffer instead of recreating every time
   uint32_t fNameWasmStr = static_cast<uint32_t>(wasmSession->createWasmStringBuffer("lingodb_udf_" + fnName));
   auto pName = wasmSession->callPyFunc2<PyObjectPtr>("PyUnicode_DecodeFSDefault", fNameWasmStr);
   if (!pName) {
      wasmSession->callPyFunc<void>("PyErr_Print");
      throw std::runtime_error{"Failed PyUnicode_DecodeFSDefault"};
   }
   auto pModule = wasmSession->callPyFunc2<PyObjectPtr>("PyImport_Import", pName);
   if (!pModule) {
      wasmSession->callPyFunc<void>("PyErr_Print");
      throw std::runtime_error{"Module not found"};
   }
   uint32_t fNameWasmStr2 = static_cast<uint32_t>(wasmSession->createWasmStringBuffer(fnName));
   auto pFunc = wasmSession->callPyFunc2<PyObjectPtr>("PyObject_GetAttrString", pModule, fNameWasmStr2);
   if (!pFunc || !wasmSession->callPyFunc2<bool>("PyCallable_Check", pFunc)) {
      wasmSession->callPyFunc<void>("PyErr_Print");
      throw std::runtime_error{"Function is not callable or null"};
   }

   PyObjectPtr resultObj = wasmSession->callPyFunc<PyObjectPtr>("PyObject_CallObject", pFunc, pArgs).at(0).of.i32;

   if (!resultObj) {
      wasmSession->callPyFunc<void>("PyErr_Print");
      throw std::runtime_error{"Error calling python method"};
   }

   return resultObj;
}

template <unsigned SIZE>
uint64_t PythonUDFRuntime::callPythonUDF(std::string fnName, std::array<uint64_t, SIZE> args) {
   PyGuard pyGuard{};
   if (Py_IsInitialized()) {
      PyStatus status;

      PyObject *pName, *pModule, *pFunc;
      PyObject *pArgs, *pValue;
      //Get file
      pName = PyUnicode_DecodeFSDefault(fnName.c_str());

      if (pName == nullptr) {
         throw std::runtime_error("Could not create/find python function");
      }
      //Import
      pModule = PyImport_Import(pName);
      if (pModule == nullptr) {
         PyErr_Print();
         throw std::runtime_error("Could not import module");
      }
      Py_DECREF(pName);
      //Get function
      pFunc = PyObject_GetAttrString(pModule, fnName.c_str());
      if (pFunc == nullptr && !PyCallable_Check(pFunc)) {
         PyErr_Print();
         throw std::runtime_error("Could not find function");
      }
      pArgs = PyTuple_New(SIZE);
      int64_t argValue;
      for (size_t i = 0; i < SIZE; i++) {
         pValue = reinterpret_cast<PyObject*>(args[i]);
         if (!pValue) {
            Py_DECREF(pArgs);
            Py_DECREF(pModule);
            throw std::runtime_error("Cannot convert argument");
         }
         PyTuple_SetItem(pArgs, i, pValue);
      }

      pValue = PyObject_CallObject(pFunc, pArgs);
      Py_DECREF(pArgs);
      Py_DECREF(pFunc);
      Py_DECREF(pModule);

      if (!pValue) {
         Py_DECREF(pFunc);
         Py_DECREF(pModule);
         PyErr_Print();
         throw std::runtime_error("Function did not return anything");
      }
      return reinterpret_cast<size_t>(pValue);
   } else {
      throw std::runtime_error("Python not initialized");
   }
}

uint32_t PythonUDFRuntime::callPythonUDF0(VarLen32 fnName) {
   return callPythonWASMUDF<0>(fnName.str(), {});
}

uint32_t PythonUDFRuntime::callPythonUDF1(VarLen32 fnName, PyObjectPtr arg) {
   return callPythonWASMUDF<1>(fnName.str(), {arg});
}

uint32_t PythonUDFRuntime::callPythonUDF2(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1) {
   return callPythonWASMUDF<2>(fnName.str(), {arg, arg1});
}

uint32_t PythonUDFRuntime::callPythonUDF3(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2) {
   return callPythonWASMUDF<3>(fnName.str(), {arg, arg1, arg2});
}

uint32_t PythonUDFRuntime::callPythonUDF4(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3) {
   return callPythonWASMUDF<4>(fnName.str(), {arg, arg1, arg2, arg3});
}

uint32_t PythonUDFRuntime::callPythonUDF5(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4) {
   return callPythonWASMUDF<5>(fnName.str(), {arg, arg1, arg2, arg3, arg4});
}

uint32_t PythonUDFRuntime::callPythonUDF6(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5) {
   return callPythonWASMUDF<6>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5});
}

uint32_t PythonUDFRuntime::callPythonUDF7(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6) {
   return callPythonWASMUDF<7>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5, arg6});
}

uint32_t PythonUDFRuntime::callPythonUDF8(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7) {
   return callPythonWASMUDF<8>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7});
}

uint32_t PythonUDFRuntime::callPythonUDF9(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7, PyObjectPtr arg8) {
   return callPythonWASMUDF<9>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8});
}

uint32_t PythonUDFRuntime::callPythonUDF10(VarLen32 fnName, PyObjectPtr arg, PyObjectPtr arg1, PyObjectPtr arg2, PyObjectPtr arg3, PyObjectPtr arg4, PyObjectPtr arg5, PyObjectPtr arg6, PyObjectPtr arg7, PyObjectPtr arg8, PyObjectPtr arg9) {
   return callPythonWASMUDF<10>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9});
}

PythonUDFRuntime::PyObjectPtr PythonUDFRuntime::int64ToPythonLong(int64_t value) {
   assert(wasm_runtime_thread_env_inited());
   auto wasmSession = lingodb::wasm::WASM::localWasmSessions[scheduler::currentWorkerId()];
   assert(wasmSession);
   assert(wasmSession->callPyFunc2<bool>("Py_IsInitialized"));
   return wasmSession->callPyFunc2<PyObjectPtr>("PyLong_FromLongLong", value);
}
PythonUDFRuntime::PyObjectPtr PythonUDFRuntime::int32ToPythonInt(int32_t value) {
   initPythonWASMCall();
   auto wasmSession = lingodb::wasm::WASM::localWasmSessions[scheduler::currentWorkerId()];
   assert(wasmSession);
   assert(wasmSession->callPyFunc2<bool>("Py_IsInitialized"));
   return wasmSession->callPyFunc2<PyObjectPtr>("PyLong_FromLong", value);
}
PythonUDFRuntime::PyObjectPtr PythonUDFRuntime::floatToPythonFloat(float value) {
   initPythonWASMCall();
   auto wasmSession = lingodb::wasm::WASM::localWasmSessions[scheduler::currentWorkerId()];
   assert(wasmSession);
   assert(wasmSession->callPyFunc2<bool>("Py_IsInitialized"));

   // Workaround: Convert float to string and parse in Python to avoid ABI/bit-width mismatches
   // (e.g. WASM passing f32 but Python expecting f64)
   double x = 11001.1;
   auto testObj = wasmSession->callPyFunc2<PyObjectPtr>("PyFloat_FromDouble",  x);


   //TODO Find a better solution
   std::string sVal = std::to_string(value);
   auto sValWasmPtr = static_cast<uint32_t>(wasmSession->createWasmStringBuffer(sVal));

   auto pStr = wasmSession->callPyFunc2<PyObjectPtr>("PyUnicode_DecodeFSDefault", sValWasmPtr);
   if (!pStr) {
      throw std::runtime_error("Failed to create python string for float");
   }
   // Use PyFloat_FromString to get a valid float object
   auto obj = wasmSession->callPyFunc2<PyObjectPtr>("PyFloat_FromString", pStr);
   return testObj;
}
PythonUDFRuntime::PyObjectPtr PythonUDFRuntime::stringToPythonString(VarLen32 value) {
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

uint64_t PythonUDFRuntime::pythonLongToInt64(uint64_t pyObj) {
   initPythonWASMCall();
   auto wasmSession = lingodb::wasm::WASM::localWasmSessions[scheduler::currentWorkerId()];
   assert(wasmSession);
   assert(wasmSession->callPyFunc2<bool>("Py_IsInitialized"));
   return wasmSession->callPyFunc2<uint64_t>("PyLong_AsLongLong", pyObj);
}
int32_t PythonUDFRuntime::pythonIntToInt32(uint64_t pyObj) {
   wasm_runtime_init_thread_env();
   initPythonWASMCall();
   auto wasmSession = lingodb::wasm::WASM::localWasmSessions[scheduler::currentWorkerId()];
   assert(wasmSession);
   assert(wasmSession->callPyFunc2<bool>("Py_IsInitialized"));
   return wasmSession->callPyFunc2<int32_t>("PyLong_AsLong", pyObj);
}
float PythonUDFRuntime::pythonFloatToFloat(uint32_t pyObj) {
   initPythonWASMCall();
   auto wasmSession = lingodb::wasm::WASM::localWasmSessions[scheduler::currentWorkerId()];
   assert(wasmSession);
   assert(wasmSession->callPyFunc2<bool>("Py_IsInitialized"));
   auto result = wasmSession->callPyFunc<double>("PyFloat_AsDouble", pyObj).at(0);
   switch (result.kind) {
      case WASM_F64:
         return result.of.f64;
         case WASM_F32:
         return result.of.f32;
      default: throw (std::runtime_error("Python not initialized"));
   }

}
VarLen32 PythonUDFRuntime::pythonStringToString(uint64_t pyObj) {
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
}
} // namespace lingodb::runtime
