#include "lingodb/runtime/UDFRuntime.h"
#include <Python.h>
#include <iostream>
uint64_t lingodb::runtime::UDFRuntime::callPythonUDF(int64_t arg) {
   if (Py_IsInitialized()) {
      PyStatus status;

      PyConfig config;
      PyConfig_InitPythonConfig(&config);
      status = PyConfig_Read(&config);
      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }
      config.module_search_paths_set = 1;

      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }

      PyObject* sysPath = PySys_GetObject("path");
      PyObject* pyDirObj = PyUnicode_DecodeFSDefault("/home/bachmaier/projects/lingo-db/resources/data/uni-udf/udf");
      PyList_Append(sysPath, pyDirObj);
      Py_DECREF(pyDirObj);

      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }

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
      pFunc = PyObject_GetAttrString(pModule, "addone");
      if (pFunc == nullptr && !PyCallable_Check(pFunc)) {
         PyErr_Print();
         throw std::runtime_error("Could not find function");
      }
      pArgs = PyTuple_New(1);
        for (size_t i = 0; i < 1; i++) {
           //Currently only support integer arguments
           int64_t argValue;

           PyObject* pValue = PyLong_FromLong(1);

           PyObject* pValue2 = reinterpret_cast<PyObject*>(arg);
           if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                throw std::runtime_error("Cannot convert argument");
           }
           PyTuple_SetItem(pArgs, i, pValue2);
        }


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

uint64_t lingodb::runtime::UDFRuntime::callPythonUDF2(VarLen32 fnName, int64_t arg, int64_t arg1) {
   if (Py_IsInitialized()) {
      PyStatus status;

      PyConfig config;
      PyConfig_InitPythonConfig(&config);
      status = PyConfig_Read(&config);
      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }
      config.module_search_paths_set = 1;

      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }

      PyObject* sysPath = PySys_GetObject("path");
      PyObject* pyDirObj = PyUnicode_DecodeFSDefault("/home/bachmaier/projects/lingo-db/resources/data/uni-udf/udf");
      PyList_Append(sysPath, pyDirObj);
      Py_DECREF(pyDirObj);

      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }

      PyObject *pName, *pModule, *pFunc;
      PyObject *pArgs, *pValue;
      //Get file
      pName = PyUnicode_DecodeFSDefault(fnName.str().c_str());

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
      pArgs = PyTuple_New(2);
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

      pValue = PyObject_CallObject(pFunc, pArgs);
      Py_DECREF(pArgs);
      if (!pValue) {
         Py_DECREF(pFunc);
         Py_DECREF(pModule);
         PyErr_Print();
         throw std::runtime_error("Function did not return anything");
      }
      return reinterpret_cast<uint64_t>(pValue);
   } else {
      throw std::runtime_error("Python not initialized");
      return -1;
   }
   return 19855;
}

uint64_t lingodb::runtime::UDFRuntime::callPythonUDF4(VarLen32 fnName, int64_t arg, int64_t arg1, int64_t arg2, int64_t arg3) {
   if (Py_IsInitialized()) {
      PyStatus status;

      PyConfig config;
      PyConfig_InitPythonConfig(&config);
      status = PyConfig_Read(&config);
      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }
      config.module_search_paths_set = 1;

      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }

      PyObject* sysPath = PySys_GetObject("path");
      PyObject* pyDirObj = PyUnicode_DecodeFSDefault("/home/bachmaier/projects/lingo-db/resources/data/uni-udf/udf");
      PyList_Append(sysPath, pyDirObj);
      Py_DECREF(pyDirObj);

      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }

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
uint64_t lingodb::runtime::UDFRuntime::int32ToPythonInt(int32_t value){
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
   if (Py_IsInitialized()) {
      PyObject* pyValue = PyFloat_FromDouble(value);
      if (!pyValue) {
         throw std::runtime_error("Could not convert float to python float");
      }
   } else {
      throw std::runtime_error("Python not initialized");
   }


}
uint64_t lingodb::runtime::UDFRuntime::doubleToPythonDouble(double value) {
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

uint64_t lingodb::runtime::UDFRuntime::pythonLongToInt64(uint64_t pyObj){
   if (!Py_IsInitialized()) {
      throw std::runtime_error("Python not initialized");
   }
   PyObject* pValue = reinterpret_cast<PyObject*>(pyObj);
   if (PyLong_Check(pValue)) {
      int64_t value = PyLong_AsLongLong(pValue);
      return value;
   } else {
      throw std::runtime_error("Provided object is not a python long");
   }
}
int32_t lingodb::runtime::UDFRuntime::pythonIntToInt32(uint64_t pyObj){
   if (!Py_IsInitialized()) {
      throw std::runtime_error("Python not initialized");
   }
   PyObject* pValue = reinterpret_cast<PyObject*>(pyObj);
   if (PyLong_Check(pValue)) {
      int32_t value = PyLong_AsLong(pValue);
      return value;
   } else {
      throw std::runtime_error("Provided object is not a python long");
   }
}
double lingodb::runtime::UDFRuntime::pythonDoubleToDouble(uint64_t pyObj){
   if (!Py_IsInitialized()) {
      throw std::runtime_error("Python not initialized");
   }
   PyObject* pValue = reinterpret_cast<PyObject*>(pyObj);
   if (PyFloat_Check(pValue)) {
      double value = PyFloat_AsDouble(pValue);
      return value;
   } else if (PyLong_Check(pValue)) {
      long value = PyLong_AsLong(pValue);
      return static_cast<double>(value);
   } else {
      throw std::runtime_error("Provided object is not a python double");
   }
}
