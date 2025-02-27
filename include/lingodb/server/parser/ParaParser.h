//
// Created by mor on 06.01.25.
//

#pragma once

#include <string>

#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "lingodb/compiler/frontend/SQL/Parser.h"
#include <lingodb/runtime/Session.h>
#include <lingodb/server/Statement.h>

namespace server {
class ParaParser {
   public:
   ParaParser() = delete;
   ParaParser(const std::shared_ptr<lingodb::runtime::Session>& session);
   std::shared_ptr<StatementInformation> getStatementInformation(std::string sql);

   private:
   StatementType getStatementType(lingodb::compiler::frontend::sql::Parser& translator);
   std::unique_ptr<std::vector<std::shared_ptr<lingodb::runtime::Relation>>> getUsedRelationsFromSelectStatement(SelectStmt* selectStatement);

   std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> findParasInSelectStatement(SelectStmt* selectStatement);
   std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> getParas(std::string sql);

   private:
   std::shared_ptr<lingodb::runtime::Session> session;
   std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> findParas(Node* currentNode);
   std::string fieldsToString(List* fields);
   std::shared_ptr<arrow::Field> getFieldOfColumn(std::string columnName);
};
}
