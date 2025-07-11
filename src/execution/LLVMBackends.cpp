#include "lingodb/execution/LLVMBackends.h"

#include "lingodb/compiler/Conversion/UtilToLLVM/Passes.h"
#include "lingodb/compiler/Dialect/util/FunctionHelper.h"
#include "lingodb/execution/BackendPasses.h"
#include "lingodb/execution/Error.h"
#include "lingodb/execution/Instrumentation.h"
#include "lingodb/runtime/GPU/Properties.h"
#include "lingodb/utility/Setting.h"
#include "lingodb/utility/Tracer.h"

#include "mlir/Conversion/ControlFlowToLLVM/ControlFlowToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVMPass.h"
#include "mlir/Conversion/SCFToControlFlow/SCFToControlFlow.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/InitAllPasses.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Support/FileUtilities.h"
#include "mlir/Target/LLVMIR/Dialect/Builtin/BuiltinToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"
#include "mlir/Target/LLVMIR/Import.h"

#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/ExecutionEngine/ObjectCache.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include <csignal>
#include <filesystem>
#include <fstream>
#include <regex>

#include <dlfcn.h>
#include <spawn.h>
#include <unistd.h>

// should be declared as a part of unistd.h, but fails on macos 15
extern char** environ;

namespace {
namespace utility = lingodb::utility;
utility::Tracer::Event execution("Execution", "run");

utility::Tracer::Event llvmCodeGen("Compilation", "LLVMCodeGen");
utility::Tracer::Event llvmOpt("Compilation", "LLVMOptPasses");

utility::GlobalSetting<bool> runLoweringPasses("system.compilation.llvm_lowering", true);
utility::GlobalSetting<std::string> perfFile("system.execution.perf_file", "perf.data");
utility::GlobalSetting<std::string> perfBinary("system.execution.perf_binary", "/usr/bin/perf");
utility::GlobalSetting<std::string> assembler("system.compilation.assembler", "as");
utility::GlobalSetting<std::string> staticLinker("system.compilation.static_linker", "c++");

namespace {
using namespace lingodb;
using namespace lingodb::compiler::dialect;
static llvm::Error makeStringError(const llvm::Twine& message) {
   return llvm::make_error<llvm::StringError>(message.str(),
                                              llvm::inconvertibleErrorCode());
}

// inlined class from https://github.com/llvm/llvm-project/blob/11857bef8a5fdfb8ab65971c3da6593c6076ff62/mlir/lib/ExecutionEngine/ExecutionEngine.cpp#L83
// required to remove linkage to MLIRExecutionEngine which leads to shared MLIR linkage which segfaults the mlir-db-opt tool.
class SimpleObjectCache : public llvm::ObjectCache {
   public:
   void notifyObjectCompiled(const llvm::Module* m,
                             llvm::MemoryBufferRef objBuffer) override {
      cachedObjects[m->getModuleIdentifier()] = llvm::MemoryBuffer::getMemBufferCopy(
         objBuffer.getBuffer(), objBuffer.getBufferIdentifier());
   }

   std::unique_ptr<llvm::MemoryBuffer> getObject(const llvm::Module* m) override {
      auto i = cachedObjects.find(m->getModuleIdentifier());
      if (i == cachedObjects.end()) {
         return nullptr;
      }
      return llvm::MemoryBuffer::getMemBuffer(i->second->getMemBufferRef());
   }

   /// Dump cached object to output file `filename`.
   void dumpToObjectFile(llvm::StringRef filename) {
      // Set up the output file.
      std::string errorMessage;
      auto file = mlir::openOutputFile(filename, &errorMessage);
      if (!file) {
         llvm::errs() << errorMessage << "\n";
         return;
      }

      // Dump the object generated for a single module to the output file.
      assert(cachedObjects.size() == 1 && "Expected only one object entry.");
      auto& cachedObject = cachedObjects.begin()->second;
      file->os() << cachedObject->getBuffer();
      file->keep();
   }

   /// Returns `true` if cache hasn't been populated yet.
   bool isEmpty() { return cachedObjects.empty(); }

   private:
   llvm::StringMap<std::unique_ptr<llvm::MemoryBuffer>> cachedObjects;
};

// from https://github.com/llvm/llvm-project/blob/70d34e4bfd32d8518a70226d3d68398d94c1d68f/mlir/include/mlir/ExecutionEngine/ExecutionEngine.h#L57
struct LLVMBackendOptions {
   /// If `llvmModuleBuilder` is provided, it will be used to create an LLVM
   /// module from the given MLIR IR. Otherwise, a default
   /// `translateModuleToLLVMIR` function will be used to translate to LLVM IR.
   llvm::function_ref<std::unique_ptr<llvm::Module>(mlir::Operation*,
                                                    llvm::LLVMContext&)>
      llvmModuleBuilder = nullptr;

   /// If `transformer` is provided, it will be called on the LLVM module during
   /// JIT-compilation and can be used, e.g., for reporting or optimization.
   llvm::function_ref<llvm::Error(llvm::Module*)> transformer = {};

   /// `jitCodeGenOptLevel`, when provided, is used as the optimization level for
   /// target code generation.
   std::optional<llvm::CodeGenOptLevel> jitCodeGenOptLevel;

   /// If `sharedLibPaths` are provided, the underlying JIT-compilation will
   /// open and link the shared libraries for symbol resolution. Libraries that
   /// are designed to be used with the `ExecutionEngine` may implement a
   /// loading and unloading protocol: if they implement the two functions with
   /// the names defined in `kLibraryInitFnName` and `kLibraryDestroyFnName`,
   /// these functions will be called upon loading the library and upon
   /// destruction of the `ExecutionEngine`. In the init function, the library
   /// may provide a list of symbols that it wants to make available to code
   /// run by the `ExecutionEngine`. If the two functions are not defined, only
   /// symbols with public visibility are available to the executed code.
   llvm::ArrayRef<llvm::StringRef> sharedLibPaths = {};

   /// Specifies an existing `sectionMemoryMapper` to be associated with the
   /// compiled code. If none is provided, a default memory mapper that directly
   /// calls into the operating system is used.
   llvm::SectionMemoryManager::MemoryMapper* sectionMemoryMapper = nullptr;

   /// If `enableObjectCache` is set, the JIT compiler will create one to store
   /// the object generated for the given module. The contents of the cache can
   /// be dumped to a file via the `dumpToObjectFile` method.
   bool enableObjectDump = false;

   /// If enable `enableGDBNotificationListener` is set, the JIT compiler will
   /// notify the llvm's global GDB notification listener.
   bool enableGDBNotificationListener = true;

   /// If `enablePerfNotificationListener` is set, the JIT compiler will notify
   /// the llvm's global Perf notification listener.
   bool enablePerfNotificationListener = true;
};

class LLVMBackend {
   public:
   /// Name of init functions of shared libraries. If a library provides a
   /// function with this name and the one of the destroy function, this function
   /// is called upon loading the library.
   static constexpr const char* const kLibraryInitFnName =
      "__mlir_execution_engine_init";

   /// Name of destroy functions of shared libraries. If a library provides a
   /// function with this name and the one of the init function, this function is
   /// called upon destructing the `LLVMBackend`.
   static constexpr const char* const kLibraryDestroyFnName =
      "__mlir_execution_engine_destroy";

   /// Function type for init functions of shared libraries. The library may
   /// provide a list of symbols that it wants to make available to code run by
   /// the `LLVMBackend`. If the two functions are not defined, only symbols
   /// with public visibility are available to the executed code.
   using LibraryInitFn = void (*)(llvm::StringMap<void*>&);

   /// Function type for destroy functions of shared libraries.
   using LibraryDestroyFn = void (*)();

   LLVMBackend(bool enableObjectDump, bool enableGDBNotificationListener,
               bool enablePerfNotificationListener) : cache(enableObjectDump ? new SimpleObjectCache() : nullptr),
                                                      functionNames(),
                                                      gdbListener(enableGDBNotificationListener ? llvm::JITEventListener::createGDBRegistrationListener() : nullptr),
                                                      perfListener(nullptr) {
      if (enablePerfNotificationListener) {
         if (auto* listener = llvm::JITEventListener::createPerfJITEventListener())
            perfListener = listener;
         else if (auto* listener = llvm::JITEventListener::createIntelJITEventListener())
            perfListener = listener;
      }
   }

   ~LLVMBackend() {
      // Execute the global destructors from the module being processed.
      // TODO: Allow JIT deinitialize for AArch64. Currently there's a bug causing a
      // crash for AArch64 see related issue #71963.
      if (jit && !jit->getTargetTriple().isAArch64())
         llvm::consumeError(jit->deinitialize(jit->getMainJITDylib()));
      // Run all dynamic library destroy callbacks to prepare for the shutdown.
      for (LibraryDestroyFn destroy : destroyFns)
         destroy();
   }

   /// Creates an execution engine for the given MLIR IR. If TargetMachine is
   /// not provided, default TM is created (i.e. ignoring any command line flags
   /// that could affect the set-up).
   static llvm::Expected<std::unique_ptr<LLVMBackend>>
   create(mlir::Operation* op, const LLVMBackendOptions& options = {},
          std::unique_ptr<llvm::TargetMachine> tm = nullptr) {
      auto engine = std::make_unique<LLVMBackend>(
         options.enableObjectDump, options.enableGDBNotificationListener,
         options.enablePerfNotificationListener);

      std::unique_ptr<llvm::LLVMContext> ctx(new llvm::LLVMContext);
      auto llvmModule = options.llvmModuleBuilder(op, *ctx);
      if (!llvmModule)
         return makeStringError("could not convert to LLVM IR");

      // If no valid TargetMachine was passed, create a default TM ignoring any
      // input arguments from the user.
      if (!tm) {
         auto tmBuilderOrError = llvm::orc::JITTargetMachineBuilder::detectHost();
         if (!tmBuilderOrError)
            return tmBuilderOrError.takeError();

         auto tmOrError = tmBuilderOrError->createTargetMachine();
         if (!tmOrError)
            return tmOrError.takeError();
         tm = std::move(tmOrError.get());
      }

      // TODO: Currently, the LLVM module created above has no triple associated
      // with it. Instead, the triple is extracted from the TargetMachine, which is
      // either based on the host defaults or command line arguments when specified
      // (set-up by callers of this method). It could also be passed to the
      // translation or dialect conversion instead of this.
      setupTargetTripleAndDataLayout(llvmModule.get(), tm.get());

      auto dataLayout = llvmModule->getDataLayout();

      // Use absolute library path so that gdb can find the symbol table.
      llvm::SmallVector<llvm::SmallString<256>, 4> sharedLibPaths;
      transform(
         options.sharedLibPaths, std::back_inserter(sharedLibPaths),
         [](llvm::StringRef libPath) {
            llvm::SmallString<256> absPath(libPath.begin(), libPath.end());
            cantFail(llvm::errorCodeToError(llvm::sys::fs::make_absolute(absPath)));
            return absPath;
         });

      // If shared library implements custom execution layer library init and
      // destroy functions, we'll use them to register the library. Otherwise, load
      // the library as JITDyLib below.
      llvm::StringMap<void*> exportSymbols;
      llvm::SmallVector<LibraryDestroyFn> destroyFns;
      llvm::SmallVector<llvm::StringRef> jitDyLibPaths;

      for (auto& libPath : sharedLibPaths) {
         auto lib = llvm::sys::DynamicLibrary::getPermanentLibrary(
            libPath.str().str().c_str());
         void* initSym = lib.getAddressOfSymbol(kLibraryInitFnName);
         void* destroySim = lib.getAddressOfSymbol(kLibraryDestroyFnName);

         // Library does not provide call backs, rely on symbol visiblity.
         if (!initSym || !destroySim) {
            jitDyLibPaths.push_back(libPath);
            continue;
         }

         auto initFn = reinterpret_cast<LibraryInitFn>(initSym);
         initFn(exportSymbols);

         auto destroyFn = reinterpret_cast<LibraryDestroyFn>(destroySim);
         destroyFns.push_back(destroyFn);
      }
      engine->destroyFns = std::move(destroyFns);

      // Callback to create the object layer with symbol resolution to current
      // process and dynamically linked libraries.
      auto objectLinkingLayerCreator = [&](llvm::orc::ExecutionSession& session,
                                           const llvm::Triple& tt) {
         auto objectLayer = std::make_unique<llvm::orc::RTDyldObjectLinkingLayer>(
            session, [sectionMemoryMapper = options.sectionMemoryMapper]() {
               return std::make_unique<llvm::SectionMemoryManager>(sectionMemoryMapper);
            });

         // Register JIT event listeners if they are enabled.
         if (engine->gdbListener)
            objectLayer->registerJITEventListener(*engine->gdbListener);
         if (engine->perfListener)
            objectLayer->registerJITEventListener(*engine->perfListener);

         // COFF format binaries (Windows) need special handling to deal with
         // exported symbol visibility.
         // cf llvm/lib/ExecutionEngine/Orc/LLJIT.cpp LLJIT::createObjectLinkingLayer
         llvm::Triple targetTriple(llvm::Twine(llvmModule->getTargetTriple()));
         if (targetTriple.isOSBinFormatCOFF()) {
            objectLayer->setOverrideObjectFlagsWithResponsibilityFlags(true);
            objectLayer->setAutoClaimResponsibilityForObjectSymbols(true);
         }

         // Resolve symbols from shared libraries.
         for (auto& libPath : jitDyLibPaths) {
            auto mb = llvm::MemoryBuffer::getFile(libPath);
            if (!mb) {
               llvm::errs() << "Failed to create MemoryBuffer for: " << libPath
                            << "\nError: " << mb.getError().message() << "\n";
               continue;
            }
            auto& jd = session.createBareJITDylib(std::string(libPath));
            auto loaded = llvm::orc::DynamicLibrarySearchGenerator::Load(
               libPath.str().c_str(), dataLayout.getGlobalPrefix());
            if (!loaded) {
               llvm::errs() << "Could not load " << libPath << ":\n  " << loaded.takeError()
                            << "\n";
               continue;
            }
            jd.addGenerator(std::move(*loaded));
            llvm::cantFail(objectLayer->add(jd, std::move(mb.get())));
         }

         return objectLayer;
      };

      // Callback to inspect the cache and recompile on demand. This follows Lang's
      // LLJITWithObjectCache example.
      auto compileFunctionCreator = [&](llvm::orc::JITTargetMachineBuilder jtmb)
         -> llvm::Expected<std::unique_ptr<llvm::orc::IRCompileLayer::IRCompiler>> {
         if (options.jitCodeGenOptLevel)
            jtmb.setCodeGenOptLevel(*options.jitCodeGenOptLevel);
         return std::make_unique<llvm::orc::TMOwningSimpleCompiler>(std::move(tm),
                                                                    engine->cache.get());
      };

      // Create the LLJIT by calling the LLJITBuilder with 2 callbacks.
      auto jit =
         cantFail(llvm::orc::LLJITBuilder()
                     .setCompileFunctionCreator(compileFunctionCreator)
                     .setObjectLinkingLayerCreator(objectLinkingLayerCreator)
                     .setDataLayout(dataLayout)
                     .create());

      // Add a ThreadSafemodule to the engine and return.
      llvm::orc::ThreadSafeModule tsm(std::move(llvmModule), std::move(ctx));
      if (options.transformer)
         llvm::cantFail(tsm.withModuleDo(
            [&](llvm::Module& module) { return options.transformer(&module); }));
      llvm::cantFail(jit->addIRModule(std::move(tsm)));
      engine->jit = std::move(jit);

      // Resolve symbols that are statically linked in the current process.
      llvm::orc::JITDylib& mainJD = engine->jit->getMainJITDylib();
      mainJD.addGenerator(
         llvm::cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
            dataLayout.getGlobalPrefix())));

      // Build a runtime symbol map from the exported symbols and register them.
      auto runtimeSymbolMap = [&](llvm::orc::MangleAndInterner interner) {
         auto symbolMap = llvm::orc::SymbolMap();
         for (auto& exportSymbol : exportSymbols)
            symbolMap[interner(exportSymbol.getKey())] = {
               llvm::orc::ExecutorAddr::fromPtr(exportSymbol.getValue()),
               llvm::JITSymbolFlags::Exported};
         return symbolMap;
      };
      engine->registerSymbols(runtimeSymbolMap);

      engine->registerSymbols([&](llvm::orc::MangleAndInterner interner) {
         auto symbolMap = llvm::orc::SymbolMap();
         util::FunctionHelper::visitAllFunctions([&](std::string s, void* ptr) {
            symbolMap[interner(s)] = llvm::orc::ExecutorSymbolDef(llvm::orc::ExecutorAddr::fromPtr(ptr), llvm::JITSymbolFlags::Exported);
         });
         lingodb::execution::visitBareFunctions([&](std::string s, void* ptr) {
            symbolMap[interner(s)] = llvm::orc::ExecutorSymbolDef(llvm::orc::ExecutorAddr::fromPtr(ptr), llvm::JITSymbolFlags::Exported);
         });
         return symbolMap;
      });

      // Execute the global constructors from the module being processed.
      // TODO: Allow JIT initialize for AArch64. Currently there's a bug causing a
      // crash for AArch64 see related issue #71963.
      if (!engine->jit->getTargetTriple().isAArch64())
         cantFail(engine->jit->initialize(engine->jit->getMainJITDylib()));

      return std::move(engine);
   }

   /// Looks up the original function with the given name and returns a
   /// pointer to it. This is not necesarily a packed function. Propagates
   /// errors in case of failure.
   llvm::Expected<void*> lookup(llvm::StringRef name) const {
      auto expectedSymbol = jit->lookup(name);

      // JIT lookup may return an Error referring to strings stored internally by
      // the JIT. If the Error outlives the LLVMBackend, it would want have a
      // dangling reference, which is currently caught by an assertion inside JIT
      // thanks to hand-rolled reference counting. Rewrap the error message into a
      // string before returning. Alternatively, ORC JIT should consider copying
      // the string into the error message.
      if (!expectedSymbol) {
         std::string errorMessage;
         llvm::raw_string_ostream os(errorMessage);
         llvm::handleAllErrors(expectedSymbol.takeError(),
                               [&os](llvm::ErrorInfoBase& ei) { ei.log(os); });
         return makeStringError(errorMessage);
      }

      if (void* fptr = expectedSymbol->toPtr<void*>())
         return fptr;
      return makeStringError("looked up function is null");
   }

   /// Set the target triple and the data layout for the input module based on
   /// the input TargetMachine. This is implicitly done when creating the
   /// engine.
   static void setupTargetTripleAndDataLayout(llvm::Module* llvmModule,
                                              llvm::TargetMachine* tm) {
      llvmModule->setDataLayout(tm->createDataLayout());
      llvmModule->setTargetTriple(tm->getTargetTriple().getTriple());
   }

   /// Dump object code to output file `filename`.
   void dumpToObjectFile(llvm::StringRef filename) {
      if (cache == nullptr) {
         llvm::errs() << "cannot dump ExecutionEngine object code to file: "
                         "object cache is disabled\n";
         return;
      }
      cache->dumpToObjectFile(filename);
   }

   private:
   /// Register symbols with this LLVMBackend.
   void registerSymbols(llvm::function_ref<llvm::orc::SymbolMap(llvm::orc::MangleAndInterner)> symbolMap) {
      auto& mainJitDylib = jit->getMainJITDylib();
      cantFail(mainJitDylib.define(
         absoluteSymbols(symbolMap(llvm::orc::MangleAndInterner(
            mainJitDylib.getExecutionSession(), jit->getDataLayout())))));
   }

   private:
   /// Ordering of llvmContext and jit is important for destruction purposes: the
   /// jit must be destroyed before the context.
   llvm::LLVMContext llvmContext;

   /// Underlying LLJIT.
   std::unique_ptr<llvm::orc::LLJIT> jit;

   /// Underlying cache.
   std::unique_ptr<SimpleObjectCache> cache;

   /// Names of functions that may be looked up.
   std::vector<std::string> functionNames;

   /// GDB notification listener.
   llvm::JITEventListener* gdbListener;

   /// Perf notification listener.
   llvm::JITEventListener* perfListener;

   /// Destroy functions in the libraries loaded by the LLVMBackend that are
   /// called when this LLVMBackend is destructed.
   llvm::SmallVector<LibraryDestroyFn> destroyFns;
};

} // namespace

struct LLVMInfo {
   llvm::TargetMachine* targetMachine;
   llvm::DataLayout dataLayout;
   LLVMInfo() {
      auto targetTriple = llvm::sys::getDefaultTargetTriple();
      std::string error;

      // Look up the target using the target triple.
      auto* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
      if (!target) {
         llvm::errs() << "Could not find target for triple: " << targetTriple
                      << "\nError: " << error << "\n";
         assert(false);
      }
      // Create the TargetMachine.
      const auto* cpu = "generic";
      const auto* features = "";
      llvm::TargetOptions opt;
      auto rm = std::optional<llvm::Reloc::Model>();
      auto* targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, rm);
      if (!targetMachine) {
         llvm::errs() << "Could not create target machine for triple: " << targetTriple
                      << "\n";
         assert(false);
      }
      this->targetMachine = targetMachine;
      // Set the data layout.
      this->dataLayout = targetMachine->createDataLayout();
   }
   ~LLVMInfo() {
      if (targetMachine) {
         delete targetMachine;
      }
   }
};
LLVMInfo& getLLVMInfo() {
   static LLVMInfo llvmInfo{};
   return llvmInfo;
}

static bool lowerToLLVMDialect(mlir::ModuleOp& moduleOp, std::shared_ptr<execution::SnapshotState> serializationState, bool verify) {
   if (runLoweringPasses.getValue()) {
      // Retrieve the data layout from the TargetMachine.
      mlir::Attribute dataLayoutSpec = mlir::translateDataLayout(getLLVMInfo().dataLayout, moduleOp->getContext());
      moduleOp->setAttr("util.dataLayout", dataLayoutSpec);

      mlir::PassManager pm2(moduleOp->getContext());
      pm2.enableVerifier(verify);
      lingodb::execution::addLingoDBInstrumentation(pm2, serializationState);
      pm2.addPass(mlir::createConvertSCFToCFPass());
      pm2.addPass(util::createUtilToLLVMPass());
      pm2.addPass(mlir::createConvertControlFlowToLLVMPass());
      pm2.addPass(mlir::createArithToLLVMConversionPass());
      pm2.addPass(mlir::createFinalizeMemRefToLLVMConversionPass());
      pm2.addPass(mlir::createArithToLLVMConversionPass());
      pm2.addPass(mlir::createConvertFuncToLLVMPass());
      pm2.addPass(mlir::createReconcileUnrealizedCastsPass());
      pm2.addPass(mlir::createCSEPass());
      if (mlir::failed(pm2.run(moduleOp))) {
         return false;
      }
      return true;
   } else {
      return true;
   }
}

static llvm::Error performDefaultLLVMPasses(llvm::Module* module) {
   utility::Tracer::Trace trace(llvmOpt);
   llvm::legacy::FunctionPassManager funcPM(module);
   funcPM.add(llvm::createInstructionCombiningPass());
   funcPM.add(llvm::createReassociatePass());
   funcPM.add(llvm::createGVNPass());
   funcPM.add(llvm::createCFGSimplificationPass());

   funcPM.doInitialization();
   for (auto& func : *module) {
      if (!func.hasOptNone()) {
         funcPM.run(func);
      }
   }
   funcPM.doFinalization();
   return llvm::Error::success();
}

static void executeCmd(const std::string& cmd, execution::Error& error) {
   auto* pPipe = ::popen(cmd.c_str(), "r");
   if (pPipe == nullptr) {
      error.emit() << "Could not compile query module statically (Pipe could not be opened)";
      return;
   }
   std::array<char, 256> buffer;
   std::string result;
   while (not std::feof(pPipe)) {
      auto bytes = std::fread(buffer.data(), 1, buffer.size(), pPipe);
      result.append(buffer.data(), bytes);
   }
   auto rc = ::pclose(pPipe);
   if (WEXITSTATUS(rc)) {
      error.emit() << "Could not compile query module statically (Pipe could not be closed)";
      return;
   }
}

static void linkStatic(LLVMBackend* engine, execution::Error& error, execution::mainFnType& mainFunc) {
   auto currPath = std::filesystem::current_path();
   std::ofstream symbolStubFile("symbol_stubs.s");
   util::FunctionHelper::visitAllFunctions([&](std::string s, void* ptr) {
      symbolStubFile << ".globl " << s << "\n"
                     << ".set " << s << ", " << std::hex << ptr << "\n";
   });
#ifdef __APPLE__
   execution::visitBareFunctions([&](std::string s, void* ptr) {
      symbolStubFile << ".globl _" << s << "\n"
                     << ".set _" << s << ", " << std::hex << ptr << "\n";
   });
#else
   execution::visitBareFunctions([&](std::string s, void* ptr) {
      symbolStubFile << ".globl " << s << "\n"
                     << ".set " << s << ", " << std::hex << ptr << "\n";
   });
#endif
   symbolStubFile.close();

   engine->dumpToObjectFile("llvm-jit-static.o");

   std::string symbolStubCompileCmd = assembler.getValue() + " symbol_stubs.s -o symbol_stubs.o";
   executeCmd(symbolStubCompileCmd, error);

   std::string linkCmd = staticLinker.getValue() + " -shared -fPIC llvm-jit-static.o symbol_stubs.o -o llvm-jit-static.so";
   executeCmd(linkCmd, error);

   void* handle = dlopen(std::string(currPath.string() + "/llvm-jit-static.so").c_str(), RTLD_LAZY);
   const char* dlsymError = dlerror();
   if (dlsymError) {
      error.emit() << "Can not open static library: " << std::string(dlsymError);
      return;
   }
   mainFunc = reinterpret_cast<execution::mainFnType>(dlsym(handle, "main"));
   dlsymError = dlerror();
   if (dlsymError) {
      dlclose(handle);
      error.emit() << "Could not load symbol for main function: " << std::string(dlsymError);
      return;
   }
   return;
}
#if GPU_ENABLED == 1
static bool lowerToLLVMWithGPU(mlir::ModuleOp& moduleOp, bool verify) {
   LLVMInitializeNVPTXTarget();
   LLVMInitializeNVPTXTargetInfo();
   LLVMInitializeNVPTXTargetMC();
   LLVMInitializeNVPTXAsmPrinter();
   std::string error;
   auto targetTriple = llvm::sys::getDefaultTargetTriple();

   // Look up the target using the target triple.
   auto* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
   if (!target) {
      llvm::errs() << error;
      return 1;
   }

   // Create the TargetMachine.
   const auto* cpu = "generic";
   const auto* features = "";
   llvm::TargetOptions opt;
   auto rm = std::optional<llvm::Reloc::Model>();
   auto* targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, rm);

   if (!targetMachine) {
      llvm::errs() << "Could not create TargetMachine!";
      return 1;
   }

   // Retrieve the data layout from the TargetMachine.
   const llvm::DataLayout& dataLayout = targetMachine->createDataLayout();
   mlir::Attribute dataLayoutSpec = mlir::translateDataLayout(dataLayout, moduleOp->getContext());
   moduleOp->setAttr("util.dataLayout", dataLayoutSpec);

   mlir::PassManager pm2(moduleOp->getContext());
   pm2.enableVerifier(verify);

   // pm2.enableIRPrinting();

   pm2.addPass(util::createUtilToLLVMPass());
   pm2.addPass(mlir::createConvertSCFToCFPass());
   // pm2.addPass(mlir::createGpuLauchSinkIndexComputationsPass());
   pm2.addPass(mlir::createConvertIndexToLLVMPass());
   // pm2.addPass(mlir::createGpuKernelOutliningPass());
   llvm::SmallVector<std::string> linkFiles = {std::string(GPU_BC_DIR) + "/RTDeviceFuncs.bc"};
   pm2.addPass(mlir::createGpuNVVMAttachTarget(mlir::GpuNVVMAttachTargetOptions{
      .chip = lingodb::runtime::gpu::getChipStr(),
      .optLevel = 3,
      .linkLibs = linkFiles,
   }));
   pm2.addNestedPass<mlir::gpu::GPUModuleOp>(mlir::createStripDebugInfoPass());
   pm2.addNestedPass<mlir::gpu::GPUModuleOp>(mlir::createConvertGpuOpsToNVVMOps());
   pm2.addNestedPass<mlir::gpu::GPUModuleOp>(mlir::createConvertIndexToLLVMPass());
   pm2.addNestedPass<mlir::gpu::GPUModuleOp>(mlir::createReconcileUnrealizedCastsPass());
   pm2.addPass(mlir::createGpuToLLVMConversionPass());
   pm2.addPass(mlir::createGpuModuleToBinaryPass(mlir::GpuModuleToBinaryPassOptions{.cmdOptions = ""}));

   pm2.addPass(mlir::createConvertControlFlowToLLVMPass());
   pm2.addPass(mlir::createFinalizeMemRefToLLVMConversionPass());
   pm2.addPass(mlir::createArithToLLVMConversionPass());
   pm2.addPass(mlir::createReconcileUnrealizedCastsPass());
   pm2.addPass(mlir::createConvertFuncToLLVMPass());
   pm2.addPass(mlir::createReconcileUnrealizedCastsPass());
   pm2.addPass(mlir::createCSEPass());
   if (mlir::failed(pm2.run(moduleOp))) {
      return false;
   }
   return true;
}

class GPULLVMBackend : public execution::ExecutionBackend {
   void execute(mlir::ModuleOp& moduleOp, runtime::ExecutionContext* executionContext) override {
      mlir::registerBuiltinDialectTranslation(*moduleOp->getContext());
      mlir::registerLLVMDialectTranslation(*moduleOp->getContext());
      llvm::InitializeNativeTarget();
      llvm::InitializeNativeTargetAsmPrinter();
      auto startLowerToLLVM = std::chrono::high_resolution_clock::now();
      if (!lowerToLLVMWithGPU(moduleOp, verify)) {
         error.emit() << "Could not lower module to llvm dialect";
         return;
      }
      auto endLowerToLLVM = std::chrono::high_resolution_clock::now();
      timing["lowerToLLVM"] = std::chrono::duration_cast<std::chrono::microseconds>(endLowerToLLVM - startLowerToLLVM).count() / 1000.0;
      double translateToLLVMIRTime;
      auto convertFn = [&](mlir::Operation* module, llvm::LLVMContext& context) -> std::unique_ptr<llvm::Module> {
         auto startTranslationToLLVMIR = std::chrono::high_resolution_clock::now();
         auto res = translateModuleToLLVMIR(module, context, "LLVMDialectModule", false);
         auto endTranslationToLLVMIR = std::chrono::high_resolution_clock::now();
         translateToLLVMIRTime = std::chrono::duration_cast<std::chrono::microseconds>(endTranslationToLLVMIR - startTranslationToLLVMIR).count() / 1000.0;
         return res;
      };
      double llvmPassesTime;

      auto optimizeFn = [&](llvm::Module* module) -> llvm::Error {
         auto startLLVMIRPasses = std::chrono::high_resolution_clock::now();
         auto error = performDefaultLLVMPasses(module);
         auto endLLVMIRPasses = std::chrono::high_resolution_clock::now();
         llvmPassesTime = std::chrono::duration_cast<std::chrono::microseconds>(endLLVMIRPasses - startLLVMIRPasses).count() / 1000.0;
         return error;
      };
      auto startJIT = std::chrono::high_resolution_clock::now();
      // Libraries that we'll pass to the LLVMBackend for loading.
      llvm::SmallVector<llvm::StringRef, 4> requiredLibs = {CUDA_RUNTIME_LIB};

      auto tmBuilderOrError = llvm::orc::JITTargetMachineBuilder::detectHost();
      if (!tmBuilderOrError) {
         llvm::errs() << "Failed to create a JITTargetMachineBuilder for the host\n";
         assert(false && "should not happen");
      }
      auto tmOrError = tmBuilderOrError->createTargetMachine();
      if (!tmOrError) {
         llvm::errs() << "Failed to create a TargetMachine for the host\n";
         assert(false && "should not happen");
      }

      auto maybeEngine = LLVMBackend::create(moduleOp, {.llvmModuleBuilder = convertFn, .transformer = optimizeFn, .jitCodeGenOptLevel = llvm::CodeGenOptLevel::Default, .sharedLibPaths = requiredLibs, .enableObjectDump = false});
      if (!maybeEngine) {
         error.emit() << "Could not create execution engine";
         return;
      }
      auto engine = std::move(maybeEngine.get());
      auto mainFnLookupResult = engine->lookup("main");
      if (!mainFnLookupResult) {
         error.emit() << "Could not lookup main function";
         return;
      }

      auto mainFunc = reinterpret_cast<execution::mainFnType>(mainFnLookupResult.get());
      auto endJIT = std::chrono::high_resolution_clock::now();
      auto totalJITTime = std::chrono::duration_cast<std::chrono::microseconds>(endJIT - startJIT).count() / 1000.0;
      totalJITTime -= translateToLLVMIRTime;
      totalJITTime -= llvmPassesTime;

      std::vector<double> measuredTimes;
      for (size_t i = 0; i < numRepetitions; i++) {
         auto executionStart = std::chrono::high_resolution_clock::now();
         utility::Tracer::Trace trace(execution);
         mainFunc();
         trace.stop();
         auto executionEnd = std::chrono::high_resolution_clock::now();
         executionContext->reset();
         measuredTimes.push_back(std::chrono::duration_cast<std::chrono::microseconds>(executionEnd - executionStart).count() / 1000.0);
      }
      timing["toLLVMIR"] = translateToLLVMIRTime;
      timing["llvmOptimize"] = llvmPassesTime;
      timing["llvmCodeGen"] = totalJITTime;
      timing["executionTime"] = (measuredTimes.size() > 1 ? *std::min_element(measuredTimes.begin() + 1, measuredTimes.end()) : measuredTimes[0]);
   }
};
#endif

class DefaultCPULLVMBackend : public execution::ExecutionBackend {
   void execute(mlir::ModuleOp& moduleOp, runtime::ExecutionContext* executionContext) override {
      mlir::registerBuiltinDialectTranslation(*moduleOp->getContext());
      mlir::registerLLVMDialectTranslation(*moduleOp->getContext());
      llvm::InitializeNativeTarget();
      llvm::InitializeNativeTargetAsmPrinter();
      auto startLowerToLLVM = std::chrono::high_resolution_clock::now();
      if (!lowerToLLVMDialect(moduleOp, getSerializationState(), verify)) {
         error.emit() << "Could not lower module to llvm dialect";
         return;
      }
      auto endLowerToLLVM = std::chrono::high_resolution_clock::now();
      timing["lowerToLLVM"] = std::chrono::duration_cast<std::chrono::microseconds>(endLowerToLLVM - startLowerToLLVM).count() / 1000.0;
      double translateToLLVMIRTime;
      auto convertFn = [&](mlir::Operation* module, llvm::LLVMContext& context) -> std::unique_ptr<llvm::Module> {
         auto startTranslationToLLVMIR = std::chrono::high_resolution_clock::now();
         auto res = translateModuleToLLVMIR(module, context, "LLVMDialectModule", false);
         auto endTranslationToLLVMIR = std::chrono::high_resolution_clock::now();
         translateToLLVMIRTime = std::chrono::duration_cast<std::chrono::microseconds>(endTranslationToLLVMIR - startTranslationToLLVMIR).count() / 1000.0;
         return res;
      };
      double llvmPassesTime;

      auto optimizeFn = [&](llvm::Module* module) -> llvm::Error {
         auto startLLVMIRPasses = std::chrono::high_resolution_clock::now();
         auto error = performDefaultLLVMPasses(module);
         auto endLLVMIRPasses = std::chrono::high_resolution_clock::now();
         llvmPassesTime = std::chrono::duration_cast<std::chrono::microseconds>(endLLVMIRPasses - startLLVMIRPasses).count() / 1000.0;
         return error;
      };
      auto startJIT = std::chrono::high_resolution_clock::now();
      utility::Tracer::Trace traceCodeGen(llvmCodeGen);

      auto maybeEngine = LLVMBackend::create(moduleOp, {.llvmModuleBuilder = convertFn, .transformer = optimizeFn, .jitCodeGenOptLevel = llvm::CodeGenOptLevel::Default, .enableObjectDump = false});
      if (!maybeEngine) {
         error.emit() << "Could not create execution engine";
         return;
      }
      auto engine = std::move(maybeEngine.get());

      auto mainFnLookupResult = engine->lookup("main");
      if (!mainFnLookupResult) {
         error.emit() << "Could not lookup main function";
         return;
      }

      auto mainFunc = reinterpret_cast<execution::mainFnType>(mainFnLookupResult.get());
      traceCodeGen.stop();
      auto endJIT = std::chrono::high_resolution_clock::now();
      auto totalJITTime = std::chrono::duration_cast<std::chrono::microseconds>(endJIT - startJIT).count() / 1000.0;
      totalJITTime -= translateToLLVMIRTime;
      totalJITTime -= llvmPassesTime;

      auto executionStart = std::chrono::high_resolution_clock::now();
      utility::Tracer::Trace trace(execution);
      mainFunc();
      trace.stop();
      auto executionEnd = std::chrono::high_resolution_clock::now();

      timing["toLLVMIR"] = translateToLLVMIRTime;
      timing["llvmOptimize"] = llvmPassesTime;
      timing["llvmCodeGen"] = totalJITTime;
      timing["executionTime"] = (std::chrono::duration_cast<std::chrono::microseconds>(executionEnd - executionStart).count() / 1000.0);
   }
};
std::optional<std::string> extractSingleResultName(llvm::StringRef line) {
   size_t eq = line.find('=');
   if (eq == llvm::StringRef::npos)
      return std::nullopt;

   llvm::StringRef lhs = line.take_front(eq);
   static const std::regex ssaRegex(R"(%[\w\d._]+)");
   std::string lhsStr = lhs.str();

   std::sregex_iterator begin(lhsStr.begin(), lhsStr.end(), ssaRegex), end;
   if (begin != end)
      return begin->str(); // take the first match
   return std::nullopt;
}
static void addDebugInfo(mlir::ModuleOp module, std::string lastSnapShotFile, bool addDbgValues) {
   std::ifstream file(lastSnapShotFile);
   std::string line;
   std::vector<std::string> lines;
   while (std::getline(file, line))
      lines.push_back(line);

   auto fileAttr = mlir::LLVM::DIFileAttr::get(module->getContext(), lastSnapShotFile, std::filesystem::current_path().string());
   auto compileUnitAttr = mlir::LLVM::DICompileUnitAttr::get(mlir::DistinctAttr::create(mlir::UnitAttr::get(module->getContext())), static_cast<uint8_t>(llvm::dwarf::DW_LANG_C), fileAttr, mlir::StringAttr::get(module->getContext(), "LingoDB"), true, mlir::LLVM::DIEmissionKind::Full);
   module->walk([&](mlir::LLVM::LLVMFuncOp funcOp) {
      mlir::DistinctAttr id;
      mlir::LLVM::DICompileUnitAttr compileUnitAt;
      auto subprogramFlags = mlir::LLVM::DISubprogramFlags::Optimized;
      if (!funcOp.isExternal()) {
         id = mlir::DistinctAttr::create(mlir::UnitAttr::get(module.getContext()));
         subprogramFlags = subprogramFlags | mlir::LLVM::DISubprogramFlags::Definition;
         compileUnitAt = compileUnitAttr;
      }
      auto subroutineType = mlir::LLVM::DISubroutineTypeAttr::get(module->getContext(), {});
      auto subProgramAttr = mlir::LLVM::DISubprogramAttr::get(module->getContext(), id, compileUnitAt, fileAttr, funcOp.getNameAttr(), funcOp.getNameAttr(), fileAttr, 0, 0, subprogramFlags, subroutineType, {}, {});
      funcOp->setLoc(mlir::FusedLocWith<mlir::LLVM::DIScopeAttr>::get(funcOp->getLoc(), subProgramAttr, module->getContext()));
      if (addDbgValues) {
         funcOp.walk([&](mlir::Operation* op) {
            if (mlir::isa<mlir::LLVM::DbgValueOp>(op) || mlir::isa<mlir::LLVM::DbgDeclareOp>(op))
               return; // skip debug ops
            if (auto loc = mlir::dyn_cast<mlir::FileLineColLoc>(op->getLoc())) {
               if (loc.getFilename().str() == lastSnapShotFile && loc.getLine() > 0 && loc.getLine() <= lines.size() && op->getNumResults() == 1) {
                  // We have a line number in the snapshot file, so we can extract the result name
                  // from the line and set it as a debug info.
                  auto resultNameOpt = extractSingleResultName(lines[loc.getLine() - 1]);
                  auto resultType = op->getResult(0).getType();
                  if (resultNameOpt) {
                     std::string resultName = "v" + resultNameOpt.value().substr(1); // remove the leading %
                     mlir::OpBuilder builder(op);
                     builder.setInsertionPointAfter(op);
                     mlir::LLVM::DITypeAttr typeAttr;
                     if (resultType.isInteger()) {
                        typeAttr = mlir::LLVM::DIBasicTypeAttr::get(builder.getContext(), llvm::dwarf::DW_TAG_base_type, "int", resultType.getIntOrFloatBitWidth(), llvm::dwarf::DW_ATE_signed);
                     }
                     if (resultType.isF32() || resultType.isF64()) {
                        typeAttr = mlir::LLVM::DIBasicTypeAttr::get(builder.getContext(), llvm::dwarf::DW_TAG_base_type, "float", resultType.getIntOrFloatBitWidth(), llvm::dwarf::DW_ATE_float);
                     }
                     if (auto ptrType = mlir::dyn_cast<mlir::LLVM::LLVMPointerType>(resultType)) {
                        auto baseIntType = mlir::LLVM::DIBasicTypeAttr::get(builder.getContext(), llvm::dwarf::DW_TAG_base_type, "void", 8, llvm::dwarf::DW_ATE_signed);
                        typeAttr = mlir::LLVM::DIDerivedTypeAttr::get(builder.getContext(), llvm::dwarf::DW_TAG_pointer_type, builder.getStringAttr("pointer"), baseIntType, 64, 0, 0, {}, mlir::LLVM::DINodeAttr());
                     }
                     if (typeAttr) {
                        auto variableAttr = mlir::LLVM::DILocalVariableAttr::get(builder.getContext(), subProgramAttr, builder.getStringAttr(resultName), fileAttr, loc.getLine(), 0, 0, typeAttr, mlir::LLVM::DIFlags{});
                        builder.create<mlir::LLVM::DbgValueOp>(loc, op->getResult(0), variableAttr);
                     }
                  }
               }
            }
         });
      }
   });
}
class CPULLVMDebugBackend : public execution::ExecutionBackend {
   void execute(mlir::ModuleOp& moduleOp, runtime::ExecutionContext* executionContext) override {
      mlir::registerBuiltinDialectTranslation(*moduleOp->getContext());
      mlir::registerLLVMDialectTranslation(*moduleOp->getContext());
      llvm::InitializeNativeTarget();
      llvm::InitializeNativeTargetAsmPrinter();
      auto startLowerToLLVM = std::chrono::high_resolution_clock::now();
      if (!lowerToLLVMDialect(moduleOp, getSerializationState(), verify)) {
         error.emit() << "Could not lower module to llvm dialect";
         return;
      }
      auto endLowerToLLVM = std::chrono::high_resolution_clock::now();
      if (auto moduleFileLineLoc = mlir::dyn_cast_or_null<mlir::FileLineColLoc>(moduleOp.getLoc())) {
         addDebugInfo(moduleOp, moduleFileLineLoc.getFilename().str(), true);
      }
      timing["lowerToLLVM"] = std::chrono::duration_cast<std::chrono::microseconds>(endLowerToLLVM - startLowerToLLVM).count() / 1000.0;
      auto convertFn = [&](mlir::Operation* module, llvm::LLVMContext& context) -> std::unique_ptr<llvm::Module> {
         return translateModuleToLLVMIR(module, context, "LLVMDialectModule", true);
      };
      //do not optimize in debug mode
      auto optimizeFn = [&](llvm::Module* module) -> llvm::Error { return llvm::Error::success(); };
      //first step: use LLVMBackend
      auto maybeEngine = LLVMBackend::create(moduleOp, {.llvmModuleBuilder = convertFn, .transformer = optimizeFn, .jitCodeGenOptLevel = llvm::CodeGenOptLevel::Default, .enableObjectDump = true});
      if (!maybeEngine) {
         error.emit() << "Could not create execution engine";
         return;
      }
      auto engine = std::move(maybeEngine.get());
      auto mainFnLookupResult = engine->lookup("main");
      if (!mainFnLookupResult) {
         error.emit() << "Could not lookup main function";
         return;
      }

      execution::mainFnType mainFunc;
      linkStatic(engine.get(), error, mainFunc);
      if (error) {
         return;
      }

      auto executionStart = std::chrono::high_resolution_clock::now();
      mainFunc();
      auto executionEnd = std::chrono::high_resolution_clock::now();

      timing["executionTime"] = std::chrono::duration_cast<std::chrono::microseconds>(executionEnd - executionStart).count() / 1000.0;
   }
};

class CPULLVMProfilingBackend : public execution::ExecutionBackend {
   pid_t runPerfRecord() {
      pid_t childPid = 0;
      auto parentPid = std::to_string(getpid());
      auto perfBinaryStr = perfBinary.getValue();
      auto perfFileStr = perfFile.getValue();
      const char* argV[] = {perfBinaryStr.c_str(), "record", "-o", perfFileStr.c_str(), "-c", "5000", "-p", parentPid.c_str(), nullptr};
      auto status = posix_spawn(&childPid, perfBinaryStr.c_str(), nullptr, nullptr, const_cast<char**>(argV), environ);
      sleep(10);
      if (status != 0)
         error.emit() << "Launching of perf failed" << status;
      return childPid;
   }
   void execute(mlir::ModuleOp& moduleOp, runtime::ExecutionContext* executionContext) override {
      mlir::registerBuiltinDialectTranslation(*moduleOp->getContext());
      mlir::registerLLVMDialectTranslation(*moduleOp->getContext());
#ifdef __x86_64__
      LLVMInitializeX86AsmParser();
#elif defined(__arm64__) || defined(__aarch64__)
      LLVMInitializeAArch64AsmParser();
#endif
      llvm::InitializeNativeTarget();
      llvm::InitializeNativeTargetAsmPrinter();
      auto targetTriple = llvm::sys::getDefaultTargetTriple();
      std::string errorMessage;
      const auto* target = llvm::TargetRegistry::lookupTarget(targetTriple, errorMessage);
      if (!target) {
         error.emit() << "Could not lookup target";
         return;
      }

      // Initialize LLVM targets.
      llvm::InitializeNativeTarget();
      llvm::InitializeNativeTargetAsmPrinter();
      auto startLowerToLLVM = std::chrono::high_resolution_clock::now();
      if (!lowerToLLVMDialect(moduleOp, getSerializationState(), verify)) {
         error.emit() << "Could not lower module to llvm dialect";
         return;
      }
      auto endLowerToLLVM = std::chrono::high_resolution_clock::now();
      timing["lowerToLLVM"] = std::chrono::duration_cast<std::chrono::microseconds>(endLowerToLLVM - startLowerToLLVM).count() / 1000.0;
      if (auto moduleFileLineLoc = mlir::dyn_cast_or_null<mlir::FileLineColLoc>(moduleOp.getLoc())) {
         addDebugInfo(moduleOp, moduleFileLineLoc.getFilename().str(), false);
      }
      double translateToLLVMIRTime;
      auto convertFn = [&](mlir::Operation* module, llvm::LLVMContext& context) -> std::unique_ptr<llvm::Module> {
         auto startTranslationToLLVMIR = std::chrono::high_resolution_clock::now();
         auto res = translateModuleToLLVMIR(module, context, "LLVMDialectModule", true);
         auto endTranslationToLLVMIR = std::chrono::high_resolution_clock::now();
         translateToLLVMIRTime = std::chrono::duration_cast<std::chrono::microseconds>(endTranslationToLLVMIR - startTranslationToLLVMIR).count() / 1000.0;
         return res;
      };
      double llvmPassesTime;

      auto optimizeFn = [&](llvm::Module* module) -> llvm::Error {
         auto startLLVMIRPasses = std::chrono::high_resolution_clock::now();
         auto error = performDefaultLLVMPasses(module);
         auto endLLVMIRPasses = std::chrono::high_resolution_clock::now();
         llvmPassesTime = std::chrono::duration_cast<std::chrono::microseconds>(endLLVMIRPasses - startLLVMIRPasses).count() / 1000.0;
         return error;
      };
      auto startJIT = std::chrono::high_resolution_clock::now();

      auto maybeEngine = LLVMBackend::create(moduleOp, {.llvmModuleBuilder = convertFn, .transformer = optimizeFn, .jitCodeGenOptLevel = llvm::CodeGenOptLevel::Default, .enableObjectDump = true});
      if (!maybeEngine) {
         error.emit() << "Could not create execution engine";
         return;
      }
      auto engine = std::move(maybeEngine.get());
      auto mainFnLookupResult = engine->lookup("main");
      if (!mainFnLookupResult) {
         error.emit() << "Could not lookup main function";
         return;
      }

      execution::mainFnType mainFunc;
      linkStatic(engine.get(), error, mainFunc);
      if (error) {
         return;
      }
      auto endJIT = std::chrono::high_resolution_clock::now();
      auto totalJITTime = std::chrono::duration_cast<std::chrono::microseconds>(endJIT - startJIT).count() / 1000.0;
      totalJITTime -= translateToLLVMIRTime;
      totalJITTime -= llvmPassesTime;

      //start profiling
      pid_t pid = runPerfRecord();
      if (error) return;
      auto executionStart = std::chrono::high_resolution_clock::now();
      mainFunc();
      auto executionEnd = std::chrono::high_resolution_clock::now();
      //finish profiling
      kill(pid, SIGINT);
      sleep(2);

      timing["toLLVMIR"] = translateToLLVMIRTime;
      timing["llvmOptimize"] = llvmPassesTime;
      timing["llvmCodeGen"] = totalJITTime;
      timing["executionTime"] = (std::chrono::duration_cast<std::chrono::microseconds>(executionEnd - executionStart).count() / 1000.0);
   }
};

} // namespace

std::unique_ptr<execution::ExecutionBackend> execution::createDefaultLLVMBackend() {
   return std::make_unique<DefaultCPULLVMBackend>();
}
std::unique_ptr<execution::ExecutionBackend> execution::createLLVMDebugBackend() {
   return std::make_unique<CPULLVMDebugBackend>();
}
std::unique_ptr<execution::ExecutionBackend> execution::createLLVMProfilingBackend() {
   return std::make_unique<CPULLVMProfilingBackend>();
}

std::unique_ptr<execution::ExecutionBackend> execution::createGPULLVMBackend() {
#if GPU_ENABLED == 1
   return std::make_unique<GPULLVMBackend>();
#else
   return {};
#endif
}
