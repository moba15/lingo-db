#ifndef LINGODB_EXECUTION_ERROR_H
#define LINGODB_EXECUTION_ERROR_H
#include <sstream>

namespace lingodb::execution {
class Error {
   public:
   enum ErrorType {
      frontend,
      backend,
      unknown
   };

   private:
   bool present = false;
   std::stringstream message;
   ErrorType type = unknown;

   public:
   std::string getMessage() { return message.str(); }
   ErrorType getErrorType() const { return type; }
   void setErrorType(ErrorType type) { this->type = type; }
   operator bool() const {
      return present;
   }
   std::stringstream& emit() {
      present = true;
      return message;
   }
};
} // namespace lingodb::execution
#endif //LINGODB_EXECUTION_ERROR_H
