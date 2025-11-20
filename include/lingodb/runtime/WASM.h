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
   // - Outs... must be supplied explicitly
   // - Ins... are deduced from the provided inputs.
   template <typename... Out, typename... Ins>
   std::vector<wasm_val_t> callPyFunc(std::string_view funcName, Ins&&... ins) {
      wasm_function_inst_t func = getPyFunc(funcName.data());
      std::vector<wasm_val_t> args(sizeof...(Ins));
      uint32_t numArgs = 0, num_results = 0;
      serializeArgs<Out...>(args.data(), numArgs, num_results, ins...);
      std::vector<wasm_val_t> results(num_results);
      assert(numArgs == sizeof...(Ins));
      bool success = wasm_runtime_call_wasm_a(execEnv, func, num_results, results.data(), numArgs, args.data());
      if (!success) {
         /* exception is thrown if call fails */
         throw std::runtime_error{wasm_runtime_get_exception(moduleInst)};
      }
      return results;
   }

   template <typename Out, typename... Ins>
   Out callPyFunc2(std::string_view funcName, Ins&&... ins) {
      wasm_function_inst_t func = getPyFunc(funcName.data());
      std::vector<wasm_val_t> args(sizeof...(Ins));
      uint32_t numArgs = 0, num_results = 0;
      serializeArgs<Out>(args.data(), numArgs, num_results, ins...);
      std::vector<wasm_val_t> results(num_results);
      assert(numArgs == sizeof...(Ins));
      bool success = wasm_runtime_call_wasm_a(execEnv, func, num_results, results.data(), numArgs, args.data());
      if (!success) {
         /* exception is thrown if call fails */

         throw std::runtime_error{wasm_runtime_get_exception(moduleInst)};
      }
      return static_cast<Out>(results.at(0).of.i64);
   }

      uint64_t createWasmStringBuffer(std::string str) {
      void* nativeBufAddr = nullptr;

      uint64_t instBufAddr = wasm_runtime_module_malloc_internal(moduleInst, execEnv, strlen(str.c_str()) + 1, &nativeBufAddr);
      if (!nativeBufAddr) {

         throw std::runtime_error(wasm_runtime_get_exception(moduleInst));
      }
      char* nativeCharBuf = std::bit_cast<char*>(nativeBufAddr);
      memcpy(nativeCharBuf, str.c_str(), strlen(str.c_str()) + 1);
      return instBufAddr;
   }

   public:
   private:
   template <typename T>
   void packVal(void* argsVoid, uint32_t& idx, T v)
      requires(std::integral<T> && sizeof(T) == 4)
   {
      static_cast<wasm_val_t*>(argsVoid)[idx++].of.i32 = static_cast<int32_t>(v);
   }
   template <typename T>
   void packVal(void* argsVoid, uint32_t& idx, T v)
      requires(std::integral<T> && sizeof(T) == 8)
   {
      static_cast<wasm_val_t*>(argsVoid)[idx++].of.i64 = static_cast<int64_t>(v);
   }
   inline void packVal(void* argsVoid, uint32_t& idx, float v) {
      static_cast<wasm_val_t*>(argsVoid)[idx++].of.f32 = v;
   }
   inline void packVal(void* argsVoid, uint32_t& idx, double v) {
      static_cast<wasm_val_t*>(argsVoid)[idx++].of.f64 = v;
   }

   // Fallback to produce a clear compile-time error for unsupported types.
   template <typename T>
   void packVal(void* /*args*/, uint32_t& /*idx*/, T /*v*/) {
      static_assert(sizeof(T) == 0, "pack_val: unsupported argument type");
   }

   template <typename T, typename... Rest>
   int countResults() {
      int count = 0;
      if constexpr (!std::is_same_v<T, void>) count = 1;
      if constexpr (sizeof...(Rest) == 0)
         return count;
      else
         return count + countResults<Rest...>();
   }

   template <typename... Outs, typename... Ins>
   void serializeArgs(wasm_val_t* args, uint32_t& numArgs, uint32_t& numResults, Ins&&... ins) {
      numArgs = 0;
      numResults = countResults<Outs...>();
      // Use fold expression to pack each input in order
      (packVal(static_cast<void*>(args), numArgs, std::forward<Ins>(ins)), ...);
   }

   wasm_function_inst_t getPyFunc(const char* funcName) {
      wasm_function_inst_t func = wasm_runtime_lookup_function(moduleInst, funcName);
      if (!func)
         throw std::runtime_error{std::format("Function '{}' not found in module", funcName)};
      return func;
   }
};
class WASM {
   public:
   static std::vector<std::shared_ptr<WASMSession>> localWasmSessions;
   static std::weak_ptr<WASMSession> initializeWASM(std::shared_ptr<catalog::Catalog> catalog, size_t id);

   static void createWASMExecEnv();
};
} // namespace lingodb::wasm

#endif //LINGODB_WASM_H
