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
StatementHandler::StatementHandler(std::unique_ptr<StatementExecution> statementExecution, size_t handleSize, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<runtime::Session>>> sessions)
   : handleSize(handleSize), statementExecution(std::move(statementExecution)), sessions(std::move(sessions)) {}
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
   ParaParser para_parser{sessions->at("tpch")};
   auto information = para_parser.getStatementInformation(sqlStatement);
   ARROW_ASSIGN_OR_RAISE(auto sharedMemoryWrapper, util::createSharedMemory(handle))

   auto statement =
      std::make_unique<Statement>(handle, sqlStatement, information, std::move(sharedSemaphore), std::move(sharedMemoryWrapper));
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

   std::cout << "Forking (pid: " << getpid() << ")" << std::endl;
   pid_t childPid = fork();

   if (childPid == -1) { return arrow::Status::Invalid("Fork failed"); }
   if (childPid == 0) {
      std::cout << "Child: Forked (pid: " << getpid() << ")" << std::endl;
      //Childsp
      //std::this_thread::sleep_for(2s);
      auto executionContext = sessions->at("tpch")->createExecutionContext();
      auto queryExecutionConfig = createQueryExecutionConfig(execution::ExecutionMode::DEFAULT, true);
      auto executer = execution::QueryExecuter::createDefaultExecuter(std::move(queryExecutionConfig), *sessions->at("tpch"));
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
         std::cout << "Getting result of" << handle << std::endl;
         auto resultTable = executer->get_execution_context()->getResultOfType<runtime::ArrowTable>(0);
         auto table = resultTable.value()->get();

         std::cout << "Got table of" << handle << std::endl;
         ARROW_ASSIGN_OR_RAISE(const auto buffer, server::util::serializeTable(table));
         resultTable.reset();
         table.reset();
         std::cout << "Use count " << table.use_count() << std::endl;
         std::cout << "Serialized table of" << handle << std::endl;
         ARROW_ASSIGN_OR_RAISE(auto sharedMemmory, server::util::createAndCopySharedResultMemory(statementQueue.at(handle)->get_share_memory_wrapper(), std::move(buffer)));
         close(statementQueue.at(handle)->get_share_memory_wrapper().getShmFd());
         shm_unlink(handle.c_str());
      }

      std::cout << "Execution finished: " << handle << std::endl;

      if (statementQueue.find(handle) == statementQueue.end()) {
         std::cerr << "Statement with handle " << handle << " was removed during execution" << std::endl;
         return arrow::Status::Invalid("Statement with handle " + handle + " was removed during execution");
      }

      ARROW_RETURN_NOT_OK(statementQueue.at(handle)->mark_as_finished(false));

      return 0;
   } else {
      //Parent
      std::cout << "Parent: Forked child " << childPid << " (pid: " << getpid() << ")" << std::endl;
      return childPid;
   }
}
arrow::Status StatementHandler::waitAndLoadResult(std::string handle) {
   UNIQUE_LOCK_AND_RETURN_NOT_ABLE(statementQueueMutex, 10s)
   CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(statementQueue, handle)

   ARROW_RETURN_NOT_OK(statementQueue.at(handle)->waitForResult());

   if (statementQueue.at(handle)->get_information()->type == StatementType::AD_HOC_UPDATE) {
      statementQueue.at(handle)->result = 0;
   }
   std::cout << "Result found for " << handle << std::endl;
   ARROW_ASSIGN_OR_RAISE(auto buffer, util::readResultSharedMemory(statementQueue.at(handle)->get_share_memory_wrapper()));
   std::cout << "Result read for " << handle << std::endl;
   ARROW_ASSIGN_OR_RAISE(auto stream, util::deserializeTableFromBufferToStream(buffer));
   statementQueue.at(handle)->result = std::move(stream);
   return arrow::Status::OK();
}

arrow::Result<std::unique_ptr<arrow::flight::FlightDataStream>> StatementHandler::getResultStream(std::string handle) {
   UNIQUE_LOCK_AND_RETURN_NOT_ABLE(statementQueueMutex, 10s)
   CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(statementQueue, handle)

   if (statementQueue.at(handle)->get_information()->type == StatementType::AD_HOC_UPDATE) {
      return arrow::Status::Invalid("");
   }
   if (std::holds_alternative<std::unique_ptr<arrow::flight::FlightDataStream>>(statementQueue.at(handle)->result)) {
      std::cout << "Found" << std::endl;
      return std::move(std::get<std::unique_ptr<arrow::flight::FlightDataStream>>(statementQueue.at(handle)->result));
   }
   return arrow::Status::Invalid("No result found");
}

arrow::Result<std::shared_ptr<arrow::Schema>> StatementHandler::getSchemaOfStatement(std::string handle) {
   UNIQUE_LOCK_AND_RETURN_NOT_ABLE(statementQueueMutex, 10s)
   CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(statementQueue, handle)

   if (statementQueue.at(handle)->get_information()->type == StatementType::AD_HOC_UPDATE) {
      return arrow::Status::Invalid("");
   }
   if (std::holds_alternative<std::unique_ptr<arrow::flight::FlightDataStream>>(statementQueue.at(handle)->result)) {
      return std::get<std::unique_ptr<arrow::flight::FlightDataStream>>(statementQueue.at(handle)->result)->schema();
   }
   return arrow::Status::Invalid("No result found");
}
arrow::Status StatementHandler::closeStatement(std::string handle) {
   UNIQUE_LOCK_AND_RETURN_NOT_ABLE(statementQueueMutex, 10s)
   CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(statementQueue, handle)
   auto it = statementQueue.find(handle);
   statementQueue.erase(it, statementQueue.end());

   return arrow::Status::OK();
}

}