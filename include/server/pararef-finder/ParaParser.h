//
// Created by mor on 06.01.25.
//

#ifndef PARAPARSER_H
#define PARAPARSER_H

#include <string>
#include <runtime/Catalog.h>

#include "mlir/Dialect/LLVMIR/LLVMDialect.h"

#include "frontend/SQL/Parser.h"

#include <json.h>
#include <runtime/Session.h>
#include <server/Statement.h>

class ParaParser {
   public:
   ParaParser() = delete;
   ParaParser(const std::shared_ptr<runtime::Session>& session);
   void parseTestA(std::string sqlUrl);
   std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> findParasInSelectStatement(SelectStmt* selectStatement);
   std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> getParas(std::string sql);
   server::StatementType getStatementType(std::string sql);
   std::unique_ptr<std::vector<std::shared_ptr<runtime::Relation>>> getRelationsFromSelectStatement(SelectStmt* selectStatement);
   std::unique_ptr<std::vector<std::shared_ptr<runtime::Relation>>>  getRelations(std::string sql);

   private:
   std::shared_ptr<runtime::Session> session;
   std::unique_ptr<std::vector<std::shared_ptr<arrow::Field>>> findParas(Node* currentNode);
   std::string fieldsToString(List* fields);
   std::shared_ptr<arrow::Field> getFieldOfColumn(std::string columnName);
};

#endif //PARAPARSER_H
