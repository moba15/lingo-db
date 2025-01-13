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

class ParaParser {
   public:
   ParaParser() = delete;
   ParaParser(const std::shared_ptr<runtime::Session>& session);
   void parseTestA(std::string sqlUrl);
   bool findParasInSelectStatement(SelectStmt* selectStatement, std::vector<std::shared_ptr<arrow::Field>>& value1);
   std::vector<std::shared_ptr<arrow::Field>> getParas(std::string sql);

   private:
   std::shared_ptr<runtime::Session> session;
   std::vector<std::shared_ptr<arrow::Field>> findParas(Node* currentNode);
   std::string fieldsToString(List* fields);
   std::shared_ptr<arrow::Field> getFieldOfColumn(std::string columnName);
};

#endif //PARAPARSER_H
