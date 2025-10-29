#ifndef LINGODB_PYTHONUTILITY_H
#define LINGODB_PYTHONUTILITY_H
#include <Python.h>
#include <memory>
namespace lingodb::utility {
struct PythonInitializerGuard {
   PythonInitializerGuard(PyThreadState* mainThreadState);
   ~PythonInitializerGuard();
   private:
   PyThreadState* mainThreadState;
};
class PythonUtility {
   public:
   //TODO that feels wrong!
   static std::unique_ptr<PythonInitializerGuard> guard;
   static std::string pythonPath;
   static std::unique_ptr<PythonInitializerGuard>& initialize();

};
} // namespace lingodb::utility


#endif //LINGODB_PYTHONUTILITY_H
