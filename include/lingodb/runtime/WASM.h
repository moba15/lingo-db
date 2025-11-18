#ifndef LINGODB_WASM_H
#define LINGODB_WASM_H

#include "lingodb/catalog/Catalog.h"
#include "wasm_c_api_internal.h"
#include "wasm_export.h"

#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace lingodb::wasm {
class WASMSession {
   public:
   WASMSession(wasm_exec_env_t execEnv, wasm_module_inst_t moduleInst)
      : execEnv(execEnv),
        moduleInst(moduleInst) {}
   wasm_exec_env_t execEnv;
   wasm_module_inst_t moduleInst;

   uint32_t get_py_obj_addr(const char* obj_name) {
      auto global_addr = wasm_runtime_lookup_memory(moduleInst, obj_name);
      if (!global_addr) {
         wasm_val_t;
         throw std::runtime_error{std::format("Global '{}' not found in module", obj_name)};
      }
      return *(uint32_t*) wasm_memory_get_base_address(static_cast<wasm_memory_inst_t>(global_addr));
   }

   // - Outs... must be supplied explicitly
   // - Ins... are deduced from the provided inputs.
   template <typename... Out, typename... Ins>
   std::vector<wasm_val_t> call_py_func(std::string_view func_name, Ins&&... ins) {
      wasm_function_inst_t func = get_py_func(func_name.data());
      std::vector<wasm_val_t> args(sizeof...(Ins));
      uint32_t num_args = 0, num_results = 0;
      serialize_args<Out...>(args.data(), num_args, num_results, ins...);
      std::vector<wasm_val_t> results(num_results);
      assert(num_args == sizeof...(Ins));
      bool success = wasm_runtime_call_wasm_a(execEnv, func, num_results, results.data(), num_args, args.data());
      if (!success) {
         /* exception is thrown if call fails */
         throw std::runtime_error{wasm_runtime_get_exception(moduleInst)};
      }
      return results;
   }

   template <typename Out, typename... Ins>
   Out call_py_func2(std::string_view func_name, Ins&&... ins) {
      wasm_function_inst_t func = get_py_func(func_name.data());
      std::vector<wasm_val_t> args(sizeof...(Ins));
      uint32_t num_args = 0, num_results = 0;
      serialize_args<Out>(args.data(), num_args, num_results, ins...);
      std::vector<wasm_val_t> results(num_results);
      assert(num_args == sizeof...(Ins));
      bool success = wasm_runtime_call_wasm_a(execEnv, func, num_results, results.data(), num_args, args.data());
      if (!success) {
         /* exception is thrown if call fails */
         throw std::runtime_error{wasm_runtime_get_exception(moduleInst)};
      }
      return static_cast<Out>(results.at(0).of.i64);
   }

   uint64_t createWasmStringBuffer(std::string str) {
      void* nativeBufAddr = nullptr;
      uint64_t instBufAddr = wasm_runtime_module_malloc(moduleInst, strlen(str.c_str()) + 1, &nativeBufAddr);
      if (!nativeBufAddr) {
         throw std::runtime_error("wasm_runtime_module_malloc failed");
      }
      char* nativeCharBuf = std::bit_cast<char*>(nativeBufAddr);
      memcpy(nativeCharBuf, str.c_str(), strlen(str.c_str()) + 1);
      return instBufAddr;
   }

   public:
   private:
   template <typename T>
   void pack_val(void* args_void, uint32_t& idx, T v)
      requires(std::integral<T> && sizeof(T) == 4)
   {
      static_cast<wasm_val_t*>(args_void)[idx++].of.i32 = static_cast<int32_t>(v);
   }
   template <typename T>
   void pack_val(void* args_void, uint32_t& idx, T v)
      requires(std::integral<T> && sizeof(T) == 8)
   {
      static_cast<wasm_val_t*>(args_void)[idx++].of.i64 = static_cast<int64_t>(v);
   }
   inline void pack_val(void* args_void, uint32_t& idx, float v) {
      static_cast<wasm_val_t*>(args_void)[idx++].of.f32 = v;
   }
   inline void pack_val(void* args_void, uint32_t& idx, double v) {
      static_cast<wasm_val_t*>(args_void)[idx++].of.f64 = v;
   }

   // Fallback to produce a clear compile-time error for unsupported types.
   template <typename T>
   void pack_val(void* /*args*/, uint32_t& /*idx*/, T /*v*/) {
      static_assert(sizeof(T) == 0, "pack_val: unsupported argument type");
   }

   template <typename T, typename... Rest>
   int count_results() {
      int count = 0;
      if constexpr (!std::is_same_v<T, void>) count = 1;
      if constexpr (sizeof...(Rest) == 0)
         return count;
      else
         return count + count_results<Rest...>();
   }

   template <typename... Outs, typename... Ins>
   void serialize_args(wasm_val_t* args, uint32_t& num_args, uint32_t& num_results, Ins&&... ins) {
      num_args = 0;
      num_results = count_results<Outs...>();
      // Use fold expression to pack each input in order
      (pack_val(static_cast<void*>(args), num_args, std::forward<Ins>(ins)), ...);
   }

   wasm_function_inst_t get_py_func(const char* func_name) {
      wasm_function_inst_t func = wasm_runtime_lookup_function(moduleInst, func_name);
      if (!func)
         throw std::runtime_error{std::format("Function '{}' not found in module", func_name)};
      return func;
   }
};
class WASM {
   public:
   static std::vector<std::shared_ptr<WASMSession>> localWasmSessions;
   static std::weak_ptr<WASMSession> initializeWASM(std::shared_ptr<catalog::Catalog> catalog, size_t id);
};
} // namespace lingodb::wasm

#endif //LINGODB_WASM_H
