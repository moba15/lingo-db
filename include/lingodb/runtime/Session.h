#ifndef LINGODB_RUNTIME_SESSION_H
#define LINGODB_RUNTIME_SESSION_H
#include "lingodb/catalog/Catalog.h"
#include "lingodb/utility/PythonUtility.h"
#include <memory>

namespace lingodb::runtime {
class ExecutionContext;
class Session {
   std::shared_ptr<catalog::Catalog> catalog;
   std::shared_ptr<utility::PythonInitializerGuard> pythonGuard;

   public:
   Session(std::shared_ptr<catalog::Catalog> catalog, std::shared_ptr<utility::PythonInitializerGuard> pythonGuard) : catalog(catalog), pythonGuard(pythonGuard) {}
   static std::shared_ptr<Session> createSession();
   static std::shared_ptr<Session> createSession(std::string dbDir, bool eagerLoading = true);
   std::shared_ptr<catalog::Catalog> getCatalog();
   std::unique_ptr<ExecutionContext> createExecutionContext();
};
} //end namespace lingodb::runtime
#endif //LINGODB_RUNTIME_SESSION_H
