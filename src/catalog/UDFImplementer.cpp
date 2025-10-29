#include "lingodb/compiler/frontend/UDFImplementer.h"

#include "lingodb/catalog/FunctionCatalogEntry.h"
#include "lingodb/catalog/MLIRTypes.h"
#include "lingodb/catalog/TableCatalogEntry.h"
#include "lingodb/compiler/Dialect/DB/IR/RuntimeFunctions.h"
#include "lingodb/execution/Execution.h"
#include "lingodb/utility/Serialization.h"
#include "lingodb/utility/Setting.h"

#include <lingodb/execution/Backend.h>
#include <mlir/Dialect/Func/IR/FuncOps.h>
#include <mlir/Dialect/SCF/IR/SCF.h>
#include <mlir/IR/Builders.h>
#include <mlir/IR/Value.h>
#include <mlir/Parser/Parser.h>

#include <filesystem>

#include <dlfcn.h>

#include <Python.h>
namespace {
lingodb::utility::GlobalSetting<std::string> cUDFCompilerDriver("system.compilation.c_udf_compiler_driver", "cc");

class CUDFImplementer : public lingodb::catalog::MLIRUDFImplementor {
   std::string functionName;
   std::string code;
   std::vector<lingodb::catalog::Type> argumentTypes;
   lingodb::catalog::Type returnType;

   public:
   CUDFImplementer(std::string functionName, std::string code, std::vector<lingodb::catalog::Type> argumentTypes, lingodb::catalog::Type returnType) : functionName(std::move(functionName)), code(std::move(code)), argumentTypes(std::move(argumentTypes)), returnType(std::move(returnType)) {}

   mlir::Value callFunction(mlir::ModuleOp& moduleOp, mlir::OpBuilder& builder, mlir::Location loc, mlir::ValueRange args, lingodb::catalog::Catalog* catalog) override {
      //Check if function has already been added before
      bool functionExists = lingodb::catalog::FunctionCatalogEntry::getUdfFunctions().contains(functionName);
      if (!functionExists) {
         auto currPath = std::filesystem::current_path().string();
         std::string pathToCFile = "";
         std::string pathToSOFile = "";
         try {
            bool soFileAlreadyExists = false;
            //Determine path to so file and check if so file already exists in db directory
            if (catalog->getDbDir().empty()) {
               char tempSoFileTemplate[] = "/tmp/c_udf_XXXXXX";
               int soFd = mkstemp(tempSoFileTemplate);
               if (soFd == -1) {
                  throw std::runtime_error("Failed to create temporary file.");
               }
               pathToSOFile = tempSoFileTemplate;
            } else {
               std::filesystem::create_directories(catalog->getDbDir() + "/udf");
               pathToSOFile = catalog->getDbDir() + "/udf/" + functionName + ".so";
               soFileAlreadyExists = std::filesystem::exists(pathToSOFile);
            }
            if (!soFileAlreadyExists) {
               char tempCFileTemplate[] = "/tmp/c_udf_XXXXXX";
               int fd = mkstemp(tempCFileTemplate);
               if (fd == -1) {
                  throw std::runtime_error("Failed to create temporary file.");
               }
               pathToCFile = std::string(tempCFileTemplate) + ".c";
               std::filesystem::rename(tempCFileTemplate, pathToCFile);

               std::ofstream tempFile(pathToCFile, std::ios::out | std::ios::trunc);

               tempFile << "#include <stdlib.h>\n"
                           "#include <string.h>\n"
                           "#include <stdint.h>\n"
                           "#include <stdbool.h>\n";
               tempFile << code;
               tempFile.close();

#ifdef __APPLE__
               std::string cmd = cUDFCompilerDriver.getValue() + std::string(" -march=native -shared -O3 -g -gdwarf-4 -fPIC -Wl, -I ") + std::string(SOURCE_DIR) + "/include " + pathToCFile + " -o " + pathToSOFile;
#else
               std::string cmd = cUDFCompilerDriver.getValue() + std::string(" -march=native -shared -O3 -g -gdwarf-4 -fPIC -Wl,--export-dynamic -I ") + std::string(SOURCE_DIR) + "/include " + pathToCFile + " -o " + pathToSOFile;
#endif
               auto* pPipe = ::popen(cmd.c_str(), "r");
               if (pPipe == nullptr) {
                  throw std::runtime_error("Could not compile query module statically (Pipe could not be opened)");
               }
               std::array<char, 256> buffer;
               std::string result;
               while (not std::feof(pPipe)) {
                  auto bytes = std::fread(buffer.data(), 1, buffer.size(), pPipe);
                  result.append(buffer.data(), bytes);
               }
               auto rc = ::pclose(pPipe);
               if (WEXITSTATUS(rc)) {
                  throw std::runtime_error("Could not compile query module statically (Pipe could not be closed)");
               }
            }
         } catch (std::exception& e) {
            throw std::runtime_error(std::string("Error during compilation of c udf: ") + e.what());
         }

         void* handle = dlopen(std::string(pathToSOFile).c_str(), RTLD_LAZY | RTLD_GLOBAL);
         const char* dlsymError = dlerror();
         if (dlsymError) {
            throw std::runtime_error(dlsymError);
         }
         assert(reinterpret_cast<lingodb::execution::mainFnType>(dlsym(handle, functionName.c_str())));
         lingodb::catalog::FunctionCatalogEntry::UDFHandle udfHandle{handle, dlsym(handle, functionName.c_str())};
         lingodb::catalog::FunctionCatalogEntry::getUdfFunctions().insert(std::pair(functionName, udfHandle));
      }

      mlir::func::FuncOp func = moduleOp.lookupSymbol<mlir::func::FuncOp>(functionName);
      if (!func || !functionExists) {
         std::vector<mlir::Type> argMLIRTypes;
         for (auto argType : argumentTypes) {
            argMLIRTypes.push_back(argType.getMLIRTypeCreator()->createType(builder.getContext()));
         }
         auto funcType = builder.getFunctionType(mlir::TypeRange(argMLIRTypes), {returnType.getMLIRTypeCreator()->createType(builder.getContext())});
         mlir::OpBuilder::InsertionGuard guard(builder);
         builder.setInsertionPointToStart(moduleOp.getBody());
         func = builder.create<mlir::func::FuncOp>(loc, functionName, funcType);
         func.setPrivate();
      }
      return builder.create<mlir::func::CallOp>(builder.getUnknownLoc(), func, args).getResult(0);
   }
};

class PythonUDFImplementer : public lingodb::catalog::MLIRUDFImplementor {
   std::string functionName;
   std::string code;
   std::vector<lingodb::catalog::Type> argumentTypes;
   lingodb::catalog::Type returnType;

   public:
   PythonUDFImplementer(std::string functionName, std::string code, std::vector<lingodb::catalog::Type> argumentTypes, lingodb::catalog::Type returnType) : functionName(std::move(functionName)), code(std::move(code)), argumentTypes(std::move(argumentTypes)), returnType(std::move(returnType)) {}

   mlir::Value callFunction(mlir::ModuleOp& moduleOp, mlir::OpBuilder& builder, mlir::Location loc, mlir::ValueRange args, lingodb::catalog::Catalog* catalog) override {
      std::string pythonPath = "";
      if (!catalog->getDbDir().empty()) {
         pythonPath = catalog->getDbDir() + "/udf/" + functionName + ".py";
         std::filesystem::create_directories(catalog->getDbDir() + "/udf/");
      }
      if (!std::filesystem::exists(pythonPath)) {
         std::ofstream tempFile(pythonPath, std::ios::out | std::ios::trunc);

         tempFile << code;
      }

      char tempSoFileTemplate[] = "/tmp/py_udf_XXXXXX";
      int soFd = mkstemp(tempSoFileTemplate);
      if (soFd == -1) {
         throw std::runtime_error("Failed to create temporary file.");
      }


      std::vector<mlir::Value> pythonArgs;
      pythonArgs.push_back(builder.create<lingodb::compiler::dialect::db::ConstantOp>(loc, lingodb::compiler::dialect::db::StringType::get(builder.getContext()), builder.getStringAttr(functionName)));
      for (size_t i = 0; i < args.size(); i++) {
         auto currentArg = args[i];
         mlir::Value pythonArg;
         switch (argumentTypes[i].getTypeId()) {
            case lingodb::catalog::LogicalTypeId::INT: {
               auto s = mlir::dyn_cast<mlir::IntegerType>(currentArg.getType()).getWidth();
               if (s == 32) {
                  pythonArg = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, mlir::IntegerType::get(builder.getContext(), 64), "int32ToPythonInt", mlir::ValueRange({currentArg})).getResult(0);
               } else if (s == 64) {
                  pythonArg = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, mlir::IntegerType::get(builder.getContext(), 64), "int64ToPythonInt", mlir::ValueRange({currentArg})).getResult(0);
               } else {
                  throw std::runtime_error("The current type is not supported in python UDFs");
               }
               break;
            }
            case lingodb::catalog::LogicalTypeId::DOUBLE: {
               pythonArg = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, mlir::IntegerType::get(builder.getContext(), 64), "doubleToPythonDouble", mlir::ValueRange({currentArg})).getResult(0);
               break;
            }
            case lingodb::catalog::LogicalTypeId::STRING: {
               pythonArg = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, mlir::IntegerType::get(builder.getContext(), 64), "stringToPythonString", mlir::ValueRange({currentArg})).getResult(0);
               break;
            }


            default: throw std::runtime_error("The current type is not supported in python UDFs");
         }
         pythonArgs.push_back(pythonArg);
      }

      auto mlirReturnType = returnType.getMLIRTypeCreator()->createType(builder.getContext());

      std::string runtimeName = "callPythonUdf";
      runtimeName += std::to_string(argumentTypes.size());
      auto pythonResult = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, mlir::IntegerType::get(builder.getContext(), 64), runtimeName, pythonArgs).getResult(0);

      //Now convert back
      mlir::Value finalResult;
      switch (returnType.getTypeId()) {
         case lingodb::catalog::LogicalTypeId::INT: {
            auto s = mlir::dyn_cast<mlir::IntegerType>(mlirReturnType).getWidth();
            if (s == 32) {
               finalResult = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, mlir::IntegerType::get(builder.getContext(), 32), "pythonIntToInt32", mlir::ValueRange({pythonResult})).getResult(0);
            } else if (s == 64) {
               finalResult = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, mlir::IntegerType::get(builder.getContext(), 64), "pythonLongToInt64", mlir::ValueRange({pythonResult})).getResult(0);
            } else {
               throw std::runtime_error("The current type is not supported in python UDFs");
            }
            break;
         }
         case lingodb::catalog::LogicalTypeId::DOUBLE: {
            finalResult = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, mlir::Float64Type::get(builder.getContext()), "pythonDoubleToDouble", mlir::ValueRange({pythonResult})).getResult(0);
            break;
         }
         case lingodb::catalog::LogicalTypeId::CHAR:
            //Do the same as with string for now
         case lingodb::catalog::LogicalTypeId::STRING: {
            finalResult = builder.create<lingodb::compiler::dialect::db::RuntimeCall>(loc, lingodb::compiler::dialect::db::StringType::get(builder.getContext()), "pythonStringToString", mlir::ValueRange({pythonResult})).getResult(0);
            break;
         }

         default: throw std::runtime_error("The current return result type is not supported in python UDFs");
      }

      return finalResult;
   }
};

} //namespace

namespace lingodb::compiler::frontend {
std::shared_ptr<catalog::MLIRUDFImplementor> getUDFImplementer(std::shared_ptr<catalog::FunctionCatalogEntry> entry) {
   switch (entry->getEntryType()) {
      case catalog::CatalogEntry::CatalogEntryType::C_FUNCTION_ENTRY: {
         return createCUDFImplementer(entry->getName(), entry->getCode(), entry->getArgumentTypes(), entry->getReturnType());
      }
      case catalog::CatalogEntry::CatalogEntryType::PYTHON_FUNCTION_ENTRY: {
         return createPythonUDFImplementer(entry->getName(), entry->getCode(), entry->getArgumentTypes(), entry->getReturnType());
         ;
      }
      default: throw std::runtime_error("getUDFImplementer: unknown catalog entry type");
   }
}
std::shared_ptr<catalog::MLIRUDFImplementor> createCUDFImplementer(std::string funcName, std::string cCode, std::vector<catalog::Type> argumentTypes, catalog::Type returnType) {
   return std::make_shared<CUDFImplementer>(funcName, cCode, argumentTypes, returnType);
}

std::shared_ptr<catalog::MLIRUDFImplementor> createPythonUDFImplementer(std::string funcName, std::string cCode, std::vector<catalog::Type> argumentTypes, catalog::Type returnType) {
   return std::make_shared<PythonUDFImplementer>(funcName, cCode, argumentTypes, returnType);
}

int main(int argc, char* argv[]) {
   PyObject *pName, *pModule, *pFunc;
   PyObject *pArgs, *pValue;
   int i;

   if (argc < 3) {
      fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
      return 1;
   }

   Py_Initialize();
   pName = PyUnicode_DecodeFSDefault(argv[1]);
   /* Error checking of pName left out */

   pModule = PyImport_Import(pName);
   Py_DECREF(pName);

   if (pModule != NULL) {
      pFunc = PyObject_GetAttrString(pModule, argv[2]);
      /* pFunc is a new reference */

      if (pFunc && PyCallable_Check(pFunc)) {
         pArgs = PyTuple_New(argc - 3);
         for (i = 0; i < argc - 3; ++i) {
            pValue = PyLong_FromLong(atoi(argv[i + 3]));
            if (!pValue) {
               Py_DECREF(pArgs);
               Py_DECREF(pModule);
               fprintf(stderr, "Cannot convert argument\n");
               return 1;
            }
            /* pValue reference stolen here: */
            PyTuple_SetItem(pArgs, i, pValue);
         }
         pValue = PyObject_CallObject(pFunc, pArgs);
         Py_DECREF(pArgs);
         if (pValue != NULL) {
            printf("Result of call: %ld\n", PyLong_AsLong(pValue));
            Py_DECREF(pValue);
         } else {
            Py_DECREF(pFunc);
            Py_DECREF(pModule);
            PyErr_Print();
            fprintf(stderr, "Call failed\n");
            return 1;
         }
      } else {
         if (PyErr_Occurred())
            PyErr_Print();
         fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
      }
      Py_XDECREF(pFunc);
      Py_DECREF(pModule);
   } else {
      PyErr_Print();
      fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
      return 1;
   }
   if (Py_FinalizeEx() < 0) {
      return 120;
   }
   return 0;
}

} // namespace lingodb::compiler::frontend