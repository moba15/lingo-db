#include "lingodb/runtime/WASM.h"

#include "bh_platform.h"
#include "bh_read_file.h"
#include "lingodb/scheduler/Scheduler.h"
#include "lingodb/utility/PythonUtility.h"
#include "wasm_c_api_internal.h"
#include "wasm_export.h"

#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <bits/this_thread_sleep.h>
#define WASM_STACK_SIZE 16777216
#define WASM_HEAP_SIZE (16777216)
#define WASM_FILE "/home/bachmaier/projects/lingo-db/build/cpython-wasm/Python-3.14.0/cross-build/wasm32-wasip1/python.aot"
namespace lingodb::wasm {

WASMSession WASM::initializeWASM() {
   while (!wasm_runtime_thread_env_inited()) {
      wasm_runtime_init_thread_env();
   }
   char errorBuf[128] = {0};
   uint32 size;
   /* initialize the wasm runtime by default configuration */
   wasm_runtime_init();
   /* read WASM file to memory buffer */
   uint8_t* buffer = reinterpret_cast<uint8_t*>(bh_read_file_to_buffer(WASM_FILE, &size));
   if (!buffer) {
      throw std::runtime_error("Failed to read WASM file");
   }
   wasm_module_t module = wasm_runtime_load(buffer, size, errorBuf, sizeof(errorBuf));
   if (!module) {
      throw std::runtime_error(errorBuf);
   }

   //This sets the CWD
   const char* dirList[] = {
      //TODO Hardcoded
      "/home/bachmaier/projects/lingo-db/build/cpython-wasm/Python-3.14.0/cross-build/wasm32-wasip1"};
   //Maps the directories: Required as CPython would not be able to find any modules


   //TODO Hardcoded
   const char* mapDirList[]{
      "/::/home/bachmaier/projects/lingo-db/build/cpython-wasm/Python-3.14.0/",
      //Map python path
      "/lib/python3.14::/home/bachmaier/projects/lingo-db/build/cpython-wasm/Python-3.14.0/Lib"};
   //Set the environment variables (for whatever reason, these are being ignored).
   char* argv[] = {"./python.aot"};

   wasm_runtime_set_wasi_args(module,
                              dirList, 1,
                              mapDirList, 2,
                              nullptr, 0,
                              argv, 1);

   /* create an instance of the WASM module (WASM linear memory is ready) */
   wasm_module_inst_t moduleInst = wasm_runtime_instantiate(module, WASM_STACK_SIZE, WASM_HEAP_SIZE,
                                                            errorBuf, sizeof(errorBuf));

   if (!moduleInst) {
      wasm_runtime_unload(module);
      throw std::runtime_error(errorBuf);
   }
   wasm_exec_env_t execEnv = wasm_runtime_create_exec_env(moduleInst, WASM_STACK_SIZE);
   if (!execEnv) {
      throw std::runtime_error("Create exec env failed");
   }
#ifdef ASAN_ACTIVE

#endif
   WASMSession wasmSession{execEnv, moduleInst};
#if !ASAN_ACTIVE
   wasm_runtime_set_native_stack_boundary(execEnv, scheduler::getStackBoundary());
#endif
   wasmSession.callPyFunc<void>("Py_Initialize");
   return wasmSession;
}
} // namespace lingodb::wasm