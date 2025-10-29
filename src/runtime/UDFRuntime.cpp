#include "lingodb/runtime/UDFRuntime.h"
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

uint64_t lingodb::runtime::UDFRuntime::callPythonUDF(std::string fnName, std::vector<uint64_t> args) {
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
      pArgs = PyTuple_New(args.size());
      int64_t argValue;
      for (size_t i = 0; i < args.size(); i++) {
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

uint64_t lingodb::runtime::UDFRuntime::callPythonUDF2(VarLen32 fnName, uint64_t arg, uint64_t arg1) {
   return callPythonUDF(fnName.str(), {arg, arg1});
}

uint64_t lingodb::runtime::UDFRuntime::callPythonUDF4(VarLen32 fnName, int64_t arg, int64_t arg1, int64_t arg2, int64_t arg3) {
   if (Py_IsInitialized()) {
      PyObject *pName, *pModule, *pFunc;
      PyObject *pArgs, *pValue;
      //Get file
      pName = PyUnicode_DecodeFSDefault("addone");

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
      pFunc = PyObject_GetAttrString(pModule, fnName.str().c_str());
      if (pFunc == nullptr && !PyCallable_Check(pFunc)) {
         PyErr_Print();
         throw std::runtime_error("Could not find function");
      }
      pArgs = PyTuple_New(4);
      int64_t argValue;
      pValue = reinterpret_cast<PyObject*>(arg);
      if (!pValue) {
         Py_DECREF(pArgs);
         Py_DECREF(pModule);
         throw std::runtime_error("Cannot convert argument");
      }
      PyTuple_SetItem(pArgs, 0, pValue);

      pValue = reinterpret_cast<PyObject*>(arg1);
      if (!pValue) {
         Py_DECREF(pArgs);
         Py_DECREF(pModule);
         throw std::runtime_error("Cannot convert argument");
      }
      PyTuple_SetItem(pArgs, 1, pValue);

      pValue = reinterpret_cast<PyObject*>(arg2);
      if (!pValue) {
         Py_DECREF(pArgs);
         Py_DECREF(pModule);
         throw std::runtime_error("Cannot convert argument");
      }
      PyTuple_SetItem(pArgs, 2, pValue);

      pValue = reinterpret_cast<PyObject*>(arg3);
      if (!pValue) {
         Py_DECREF(pArgs);
         Py_DECREF(pModule);
         throw std::runtime_error("Cannot convert argument");
      }
      PyTuple_SetItem(pArgs, 3, pValue);

      pValue = PyObject_CallObject(pFunc, pArgs);
      Py_DECREF(pArgs);
      if (!pValue) {
         Py_DECREF(pFunc);
         Py_DECREF(pModule);
         PyErr_Print();
         throw std::runtime_error("Function did not return anything");
      }
      std::cout << "Result of call: " << PyLong_AsLong(pValue) << std::endl;
      return reinterpret_cast<uint64_t>(pValue);
   } else {
      throw std::runtime_error("Python not initialized");
      return -1;
   }
   return 19855;
}

uint64_t lingodb::runtime::UDFRuntime::int64ToPythonLong(int64_t value) {
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
uint64_t lingodb::runtime::UDFRuntime::int32ToPythonInt(int32_t value) {
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

uint64_t lingodb::runtime::UDFRuntime::floatToPythonFloat(float value) {
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
uint64_t lingodb::runtime::UDFRuntime::doubleToPythonDouble(double value) {
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

uint64_t lingodb::runtime::UDFRuntime::pythonLongToInt64(uint64_t pyObj) {
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
int32_t lingodb::runtime::UDFRuntime::pythonIntToInt32(uint64_t pyObj) {
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
double lingodb::runtime::UDFRuntime::pythonDoubleToDouble(uint64_t pyObj) {
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
