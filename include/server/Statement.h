//
// Created by mor on 10.01.25.
//

#pragma once
#include "shm/SyncHelper.h"

#include <string>
#include <arrow/result.h>
#include <arrow/table.h>
#include <arrow/util/launder.h>
namespace  server {


enum class StatementType {
   AD_HOC,
   PreparedStatement,
};

enum StatementStatus { IDLE, EXECUTING, DONE, ERROR };
class Statement {
   public:
   Statement(std::string handle, std::string sqlStatement, StatementType type,
             std::unique_ptr<util::SharedSemaphore> shared_semaphore);
   [[nodiscard]] std::string get_handle() const { return handle; }
   [[nodiscard]] std::string get_sql_statement() const { return sqlStatement; }
   [[nodiscard]] StatementStatus get_status() const { return status; }
   [[nodiscard]] StatementType get_type() const { return type; }
   [[nodiscard]] arrow::Result<std::shared_ptr<arrow::Table>> get_result() const;
   [[deprecated]] void set_result(std::unique_ptr<util::SharedMemoryWrapper> result) {}
   arrow::Status mark_as_finished(bool error);
   arrow::Status waitForResult();

   protected:
   std::string handle;
   std::string sqlStatement;
   StatementType type;
   StatementStatus status = IDLE;
   std::unique_ptr<util::SharedMemoryWrapper> result;
   std::unique_ptr<util::SharedSemaphore> sharedSemaphore;
};
}
