//
// Created by mor on 10.01.25.
//

#include "server/StatementHandler.h"

#include <mutex>
#include <execution/Execution.h>
#include <server/ipc/IPCHelper.h>
#include <server/parser/ParaParser.h>
using namespace std::chrono_literals;
namespace server {
StatementHandler::StatementHandler(std::unique_ptr<StatementExecution> statementExecution, size_t handleSize, std::shared_ptr<runtime::Session> session)
   : handleSize(handleSize), statementExecution(std::move(statementExecution)), session(session) {}
//TODO is this good practice?
arrow::Result<std::shared_ptr<StatementInformation>> StatementHandler::getStatement(std::string handle) {
   if (statementQueue.find(handle) == statementQueue.end()) {
      return arrow::Status::Invalid("Unknown handle '" + handle + "'");

   }
   return statementQueue.at(handle)->get_information();
}

arrow::Result<std::string> StatementHandler::addStatementToQueue(std::string sqlStatement) {
   if (sqlStatement.find("?") != std::string::npos) { return arrow::Status::Invalid("Invalid SQL statement"); }
   UNIQUE_LOCK_AND_RETURN_NOT_ABLE(statementQueueMutex, 10s)

   std::string handle = randomString(handleSize);
   while (statementQueue.find(handle) != statementQueue.end()) { handle = randomString(handleSize); }
   ARROW_ASSIGN_OR_RAISE(auto sharedSemaphore, util::createAndLockSharedMutex(handle))
   ParaParser para_parser{session};
   auto information = para_parser.getStatementInformation(sqlStatement);


   auto statement =
      std::make_unique<Statement>(handle, sqlStatement, information, std::move(sharedSemaphore));
   statementQueue.emplace(handle, std::move(statement));

   return arrow::Result<std::string>(handle);
}
arrow::Result<pid_t> StatementHandler::executeQeueryStatement(std::string handle, bool onlyIfQuery) {
   {
      UNIQUE_LOCK_AND_RETURN_NOT_ABLE(statementQueueMutex, 10s)
      CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(statementQueue, handle)
   }
   if (onlyIfQuery && statementQueue.at(handle)->get_information()->type != StatementType::AD_HOC_QUERY) {
      //Only execute Query statements
      return -1;
   }
   pid_t childPid = fork();

   if (childPid == -1) { return arrow::Status::Invalid("Fork failed"); }
   if (childPid == 0) {
      //Child
      //std::this_thread::sleep_for(2s);
      auto executionContext = session->createExecutionContext();
      auto queryExecutionConfig = createQueryExecutionConfig(execution::ExecutionMode::DEFAULT, true);
      auto executer = execution::QueryExecuter::createDefaultExecuter(std::move(queryExecutionConfig), *session);
      CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(statementQueue, handle)
      auto sqlStatement = statementQueue.at(handle)->get_sql_statement();
      executer->fromData(sqlStatement);
      try {
         std::cout << "Executing " << sqlStatement << std::endl;
         executer->execute();
         std::cout << "Executed " << sqlStatement << std::endl;
      } catch (const std::runtime_error& e) {
         std::cerr << e.what() << std::endl;

         if (statementQueue.find(handle) == statementQueue.end()) {
            std::cerr << "Statement with handle " << handle << " was removed during execution" << std::endl;

         } else {
            ARROW_RETURN_NOT_OK(statementQueue.at(handle)->mark_as_finished(false));
         }
         return arrow::Status::Invalid(e.what());
      }
      if (statementQueue.at(handle)->get_information()->type == StatementType::AD_HOC_QUERY) {
         auto resultTable = executer->get_execution_context()->getResultOfType<runtime::ArrowTable>(0);
         auto table = resultTable.value()->get();
         ARROW_ASSIGN_OR_RAISE(const auto buffer, server::util::serializeTable(table));
         ARROW_ASSIGN_OR_RAISE(auto sharedMemmory, server::util::createAndCopySharedResultMemory(handle, buffer));
      }

      std::cout << "Execution finished: " << handle << std::endl;

      if (statementQueue.find(handle) == statementQueue.end()) {
         std::cerr << "Statement with handle " << handle << " was removed during execution" << std::endl;
         return arrow::Status::Invalid("Statement with handle " + handle + " was removed during execution");
      }

      ARROW_RETURN_NOT_OK(statementQueue.at(handle)->mark_as_finished(false));
      std::exit(0);

      return arrow::Status::Invalid("Child process return value has to be ignored");
   } else {
      //Parent

      return childPid;
   }
}
arrow::Result<std::variant<std::unique_ptr<arrow::flight::FlightDataStream>, int>> StatementHandler::waitAndGetStatementResult(std::string handle) {
   UNIQUE_LOCK_AND_RETURN_NOT_ABLE(statementQueueMutex, 10s)
   CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(statementQueue, handle)

   ARROW_RETURN_NOT_OK(statementQueue.at(handle)->waitForResult());
   if (statementQueue.at(handle)->get_information()->type == StatementType::AD_HOC_UPDATE) {
      return 0;
   }
   std::cout << "Result found for " << handle << std::endl;
   ARROW_ASSIGN_OR_RAISE(auto buffer, util::readResultSharedMemory(handle));
   std::cout << "Result read for " << handle << std::endl;
   ARROW_ASSIGN_OR_RAISE(auto stream, util::deserializeTableFromBufferToStream(buffer));

   return std::move(stream);
}
arrow::Status StatementHandler::closeStatement(std::string handle) {
   UNIQUE_LOCK_AND_RETURN_NOT_ABLE(statementQueueMutex, 10s)
   CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(statementQueue, handle)
   auto it = statementQueue.find(handle);
   statementQueue.erase(it, statementQueue.end());

   return arrow::Status::OK();
}

}