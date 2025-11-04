#ifndef LINGODB_PYTHONUTILITY_H
#define LINGODB_PYTHONUTILITY_H

#include <format>
#include <memory>
#include <vector>
namespace lingodb::utility {

struct WASMSession {
   private:
};

struct PythonInitializerGuard;
class PythonUtility {
   public:
   //TODO that feels wrong!
   static std::shared_ptr<PythonInitializerGuard> guard;
   static std::shared_ptr<WASMSession> wasmSession;
   static std::string pythonPath;
   static std::shared_ptr<PythonInitializerGuard>& initialize();
   static std::weak_ptr<WASMSession> initializeWithWasm();

   private:
};
} // namespace lingodb::utility

#endif //LINGODB_PYTHONUTILITY_H
