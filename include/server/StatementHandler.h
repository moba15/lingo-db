//
// Created by mor on 10.01.25.
//

#pragma once
#include "Statement.h"
#include "server/StatementExecution.h"

#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <arrow/result.h>
#include <arrow/util/launder.h>
#include <runtime/ArrowTable.h>
#include <runtime/Session.h>
#define CHECK_FOR_HANDLE_IN_QUEUE_AND_RETURN(queue, handle) \
   if (queue.find(handle) == queue.end()) { return arrow::Status::Invalid("Handle not found in queue"); }
#define UNIQUE_LOCK_AND_RETURN_NOT_ABLE(mutex, timout) \
   const std::unique_lock lock{mutex, timout};         \
   if (!lock.owns_lock()) { return arrow::Status::Invalid("Not able to lock"); }
namespace server {

class StatementHandler {
   public:
   explicit StatementHandler(std::unique_ptr<StatementExecution> statementExecution, size_t handleSize = 1024);

   arrow::Result<std::string> addStatementToQueue(std::string sqlStatement);
   arrow::Result<pid_t> executeStatement(std::string handle, std::shared_ptr<runtime::Session> session);
   arrow::Result<std::shared_ptr<arrow::Buffer>> waitAndGetStatementResult(std::string handle);
   arrow::Status closeStatement(std::string handle);

   private:
   size_t handleSize;
   std::unique_ptr<StatementExecution> statementExecution;
   std::map<std::string, std::unique_ptr<Statement>> statementQueue;
   std::timed_mutex statementQueueMutex;
   static std::string randomString(int length) {
      // Characters to use for generating the string
      const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
      std::string randomString;

      // Seed the random number generator with current time
      std::srand(std::time(nullptr));

      // Generate the random string
      for (int i = 0; i < length; ++i) { randomString += characters[std::rand() % characters.size()]; }

      return randomString;
   }
};

}
