#include "lingodb/runtime/ExecutionContext.h"

#include "wasm_export.h"
#include "lingodb/runtime/WASM.h"

#include <cassert>

void lingodb::runtime::ExecutionContext::setResult(uint32_t id, uint8_t* ptr) {
   auto* context = getCurrentExecutionContext();
   assert(context);
   context->results[id] = ptr;
}
void lingodb::runtime::ExecutionContext::clearResult(uint32_t id) {
   auto* context = getCurrentExecutionContext();
   context->results.erase(id);
}

void lingodb::runtime::ExecutionContext::setTupleCount(uint32_t id, int64_t tupleCount) {
   auto* context = getCurrentExecutionContext();
   context->tupleCounts[id] = tupleCount;
}

lingodb::runtime::ExecutionContext::~ExecutionContext() {
   states.forEach([&](void* key, State value) {
      value.freeFn(value.ptr);
   });

   for (auto local : allocators) {
      for (auto a : local) {
         a.second.freeFn(a.second.ptr);
      }
   }
   allocators.clear();
   states.clear();
}

uint8_t* lingodb::runtime::ExecutionContext::allocStateRaw(size_t size) {
   auto* context = getCurrentExecutionContext();
   assert(context);
   uint8_t* ptr = static_cast<uint8_t*>(malloc(size));
   context->registerState({ptr, [](void* p) { free(p); }});
   return ptr;
}
void lingodb::runtime::ExecutionContext::setupWasm() {
   auto workerId = scheduler::currentWorkerId();
   auto& env = wasmEnvironments[workerId];
   if (env.first == nullptr) {
      wasm::WASMSession session = wasm::WASM::initializeWASM();
      env.first = session.execEnv;
      env.second = session.moduleInst;
   } else {

   }
}
void lingodb::runtime::ExecutionContext::teardownWasm(){
   //TODO teardown
}
lingodb::wasm::WASMSession lingodb::runtime::ExecutionContext::getWasmSession(){
   auto workerId = scheduler::currentWorkerId();
   auto& env = wasmEnvironments[workerId];
   assert(env.first && env.second);
   return wasm::WASMSession(static_cast<wasm_exec_env_t>(env.first), static_cast<wasm_module_inst_t>(env.second));

}

namespace {
thread_local lingodb::runtime::ExecutionContext* currentExecutionContext = nullptr;
} // end namespace
void lingodb::runtime::setCurrentExecutionContext(lingodb::runtime::ExecutionContext* context) {
   currentExecutionContext = context;
}

lingodb::runtime::ExecutionContext* lingodb::runtime::getCurrentExecutionContext() {
   assert(currentExecutionContext);
   return currentExecutionContext;
}