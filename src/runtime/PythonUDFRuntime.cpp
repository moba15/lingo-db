#include "lingodb/runtime/PythonUDFRuntime.h"
#include <iostream>
#include <Python.h>
namespace lingodb::runtime {
class PyGuard {
   public:
   PyGuard() : gstate(PyGILState_Ensure()) {}
   ~PyGuard() { PyGILState_Release(gstate); }

   private:
   PyGILState_STATE gstate;
};
template<unsigned SIZE>
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

uint64_t PythonUDFRuntime::callPythonUDF1(VarLen32 fnName, uint64_t arg) {
   return callPythonUDF<1>(fnName.str(), {arg});
}

uint64_t PythonUDFRuntime::callPythonUDF2(VarLen32 fnName, uint64_t arg, uint64_t arg1) {
   return callPythonUDF<2>(fnName.str(), {arg, arg1});
}

uint64_t PythonUDFRuntime::callPythonUDF3(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2) {
   return callPythonUDF<3>(fnName.str(), {arg, arg1, arg2});
}

uint64_t PythonUDFRuntime::callPythonUDF4(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
   return callPythonUDF<4>(fnName.str(), {arg, arg1, arg2, arg3});
}

uint64_t PythonUDFRuntime::callPythonUDF5(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
   return callPythonUDF<5>(fnName.str(), {arg, arg1, arg2, arg3, arg4});
}

uint64_t PythonUDFRuntime::callPythonUDF6(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
   return callPythonUDF<6>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5});
}

uint64_t PythonUDFRuntime::callPythonUDF7(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
   return callPythonUDF<7>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5, arg6});
}

uint64_t PythonUDFRuntime::callPythonUDF8(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7) {
   return callPythonUDF<8>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7});
}

uint64_t PythonUDFRuntime::callPythonUDF9(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7, uint64_t arg8) {
   return callPythonUDF<9>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8});
}

uint64_t PythonUDFRuntime::callPythonUDF10(VarLen32 fnName, uint64_t arg, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6, uint64_t arg7, uint64_t arg8, uint64_t arg9) {
   return callPythonUDF<10>(fnName.str(), {arg, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9});
}


uint64_t PythonUDFRuntime::int64ToPythonLong(int64_t value) {
   PyGuard pyGuard{};
   if (Py_IsInitialized()) {
      PyObject* pyValue = PyLong_FromLongLong(value);
      if (!pyValue) {
         throw std::runtime_error("Could not convert long to python int");
      }
      return reinterpret_cast<uint64_t>(pyValue);
   } else {
      throw std::runtime_error("Python not initialized");
   }
}
uint64_t PythonUDFRuntime::int32ToPythonInt(int32_t value) {
   PyGuard pyGuard{};
   if (Py_IsInitialized()) {
      PyObject* pyValue = PyLong_FromLong(value);
      if (!pyValue) {
         throw std::runtime_error("Could not convert long to python int");
      }
      return reinterpret_cast<uint64_t>(pyValue);
   } else {
      throw std::runtime_error("Python not initialized");
   }
}

uint64_t PythonUDFRuntime::floatToPythonFloat(float value) {
   PyGuard pyGuard{};
   if (Py_IsInitialized()) {
      PyObject* pyValue = PyFloat_FromDouble(value);
      if (!pyValue) {
         throw std::runtime_error("Could not convert float to python float");
      }
      return reinterpret_cast<uint64_t>(pyValue);
   } else {
      throw std::runtime_error("Python not initialized");
   }
}
uint64_t PythonUDFRuntime::doubleToPythonDouble(double value) {
   PyGuard pyGuard{};
   if (Py_IsInitialized()) {
      PyObject* pyValue = PyFloat_FromDouble(value);
      if (!pyValue) {
         throw std::runtime_error("Could not convert double to python float");
      }
      return reinterpret_cast<uint64_t>(pyValue);
   } else {
      throw std::runtime_error("Python not initialized");
   }
}

uint64_t PythonUDFRuntime::pythonLongToInt64(uint64_t pyObj) {
   PyGuard pyGuard{};
   if (!Py_IsInitialized()) {
      throw std::runtime_error("Python not initialized");
   }
   PyObject* pValue = reinterpret_cast<PyObject*>(pyObj);
   if (PyLong_Check(pValue)) {
      int64_t value = PyLong_AsLongLong(pValue);
      Py_DECREF(pValue);
      return value;
   } else {
      Py_DECREF(pValue);
      throw std::runtime_error("Provided object is not a python long");
   }
}
int32_t PythonUDFRuntime::pythonIntToInt32(uint64_t pyObj) {
   PyGuard pyGuard{};
   if (!Py_IsInitialized()) {
      throw std::runtime_error("Python not initialized");
   }
   PyObject* pValue = reinterpret_cast<PyObject*>(pyObj);
   if (PyLong_Check(pValue)) {
      int32_t value = PyLong_AsLong(pValue);
      Py_DECREF(pValue);
      return value;
   } else {
      Py_DECREF(pValue);
      throw std::runtime_error("Provided object is not a python long");
   }
}
double PythonUDFRuntime::pythonDoubleToDouble(uint64_t pyObj) {
   PyGuard pyGuard{};
   if (!Py_IsInitialized()) {
      throw std::runtime_error("Python not initialized");
   }
   PyObject* pValue = reinterpret_cast<PyObject*>(pyObj);
   if (PyFloat_Check(pValue)) {
      double value = PyFloat_AsDouble(pValue);
      Py_DECREF(pValue);
      return value;
   } else if (PyLong_Check(pValue)) {
      long value = PyLong_AsLong(pValue);
      Py_DECREF(pValue);
      return static_cast<double>(value);
   } else {
      Py_DECREF(pValue);
      throw std::runtime_error("Provided object is not a python double");
   }
}
} // namespace lingodb::runtime
