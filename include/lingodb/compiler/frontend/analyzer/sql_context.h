#pragma once
#include "lingodb/compiler/frontend/analyzer/sql_scope.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/columnref_expression.h"

#include <memory>
#include <vector>
namespace lingodb::analyzer {

class SQLContext {
   public:
   SQLContext() = default;
   std::shared_ptr<catalog::Catalog> catalog;
   std::vector<std::shared_ptr<SQLScope>> scopes;
   std::shared_ptr<SQLScope> currentScope;
   void pushNewScope();
   void popCurrentScope();
   std::pair<std::string, std::vector<catalog::Column>> findColumn(const std::string& column_name) const;
   std::pair<std::string, std::vector<catalog::Column>> findColumn(const std::string& column_name, const std::string& alias) const;
};
} // namespace lingodb::analyzer