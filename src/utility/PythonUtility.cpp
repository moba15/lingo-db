#include "lingodb/utility/PythonUtility.h"


#include <stdexcept>

#include <iostream>
#include <Python.h>

namespace lingodb::utility {
struct PythonInitializerGuard {
   PythonInitializerGuard(PyThreadState* mainThreadState);
   ~PythonInitializerGuard();

   private:
   PyThreadState* mainThreadState;
};
PythonInitializerGuard::PythonInitializerGuard(PyThreadState* mainThreadState) : mainThreadState(mainThreadState) {
}
PythonInitializerGuard::~PythonInitializerGuard() {
   assert(mainThreadState);
   PyEval_RestoreThread(mainThreadState);
   mainThreadState = nullptr;
   Py_FinalizeEx();
}
std::shared_ptr<PythonInitializerGuard> PythonUtility::guard = nullptr;
std::string PythonUtility::pythonPath = "";
std::shared_ptr<PythonInitializerGuard>& PythonUtility::initialize() {
   if (guard) {
      return guard;
   }
   char tempPythonDirTemplate[] = "/tmp/pyUdf_XXXXXX";
   char* dirName = mkdtemp(tempPythonDirTemplate);
   if (!dirName) {
      throw std::runtime_error("Failed to create temporary directory.");
   }
   pythonPath = std::string(dirName);

   //Init python
   if (!Py_IsInitialized()) {
      Py_Initialize();
      PyConfig config;
      PyConfig_InitPythonConfig(&config);
      PyStatus status = PyConfig_Read(&config);
      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }
      config.module_search_paths_set = 1;
      if (PyStatus_Exception(status)) {
         throw std::runtime_error("Could not read python config");
      }

      PyObject* sysPath = PySys_GetObject("path");
      PyObject* pyDirObj = PyUnicode_DecodeFSDefault(pythonPath.c_str());
      PyList_Append(sysPath, pyDirObj);
      Py_DECREF(pyDirObj);

      if (PyStatus_Exception(status)) {
         PyErr_Print();
         throw std::runtime_error("Could not read python config");
      }
      PyConfig_Clear(&config);

   } else {
      throw std::runtime_error("Python is already initialized");
   }
   guard = std::make_shared<PythonInitializerGuard>(PyEval_SaveThread());
   return guard;
}
std::shared_ptr<WASMSession> PythonUtility::wasmSession = nullptr;
std::weak_ptr<WASMSession> PythonUtility::initializeWithWasm() {
   throw std::runtime_error("Not impl");
}

} // namespace lingodb::utility