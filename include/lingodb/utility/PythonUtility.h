#ifndef LINGODB_PYTHONUTILITY_H
#define LINGODB_PYTHONUTILITY_H
#include <memory>
namespace lingodb::utility {
struct PythonInitializerGuard;
class PythonUtility {
   public:
   //TODO that feels wrong!
   static std::shared_ptr<PythonInitializerGuard> guard;
   static std::string pythonPath;
   static std::shared_ptr<PythonInitializerGuard>& initialize();

};
} // namespace lingodb::utility


#endif //LINGODB_PYTHONUTILITY_H
