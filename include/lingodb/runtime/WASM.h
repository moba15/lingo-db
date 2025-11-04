#ifndef LINGODB_WASM_H
#define LINGODB_WASM_H

#include "wasm_c_api_internal.h"
#include "wasm_export.h"
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
// Forward declare WASM C-API types

namespace lingodb::wasm {
struct WASMContext {
   WASMContext(wasm_exec_env_t exec_env, wasm_module_inst_t module_inst)
      : exec_env(exec_env),
        module_inst(module_inst) {}
   wasm_exec_env_t exec_env;
   wasm_module_inst_t module_inst;
};
class WASM {
   public:
   static std::shared_ptr<WASMContext> context;
   static std::shared_ptr<WASMContext> initializeWASM();

   static wasm_function_inst_t get_py_func(wasm_module_inst_t module_inst, const char* func_name);
   template <typename... Outs, typename... Ins>
   static std::vector<wasm_val_t> call_py_func(wasm_exec_env_t exec_env,
                                               wasm_module_inst_t module_inst,
                                               std::string_view func_name, Ins&&... ins);
};
} // namespace lingodb::wasm

#endif //LINGODB_WASM_H
