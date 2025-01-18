//
// Created by mor on 10.01.25.
//

#include "server/Statement.h"
namespace server {

Statement::Statement(std::string handle, std::string sqlStatement,  std::shared_ptr<StatementInformation> information,
                     std::unique_ptr<util::SharedSemaphore> sharedSemaphore)
   : handle(handle), sqlStatement(sqlStatement), information(information), sharedSemaphore(std::move(sharedSemaphore)) {}
arrow::Result<std::shared_ptr<arrow::Table>> Statement::get_result() const {
   return arrow::Status::NotImplemented("Statement not implemented");
}
arrow::Status Statement::mark_as_finished(const bool error) {
   ARROW_RETURN_NOT_OK(sharedSemaphore->post());
   std::cout << "Posted" << std::endl;
   if (error) {
      status = ERROR;
      return arrow::Status::OK();
   }
   status = DONE;
   return arrow::Status::OK();
}
arrow::Status Statement::waitForResult() {
   ARROW_RETURN_NOT_OK(sharedSemaphore->wait());
   return arrow::Status::OK();
}
}