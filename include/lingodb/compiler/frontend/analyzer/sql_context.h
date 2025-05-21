#pragma once
#include "lingodb/compiler/frontend/analyzer/sql_scope.h"
#include "lingodb/compiler/frontend/sql-parser/aggregation_node.h"
#include "lingodb/compiler/frontend/sql-parser/group_by_node.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"

#include <iomanip>
#include <memory>
#include <vector>
namespace lingodb::analyzer {

class ASTTransformContext {
   public:
   ASTTransformContext();
   std::shared_ptr<ast::AggregationNode> aggregationNode;

};

class SQLContext {
   public:
   SQLContext() = default;
   std::shared_ptr<catalog::Catalog> catalog;
   std::vector<std::shared_ptr<SQLScope>> scopes;
   std::shared_ptr<SQLScope> currentScope;
   void pushNewScope();
   void popCurrentScope();
   std::pair<std::string, std::vector<catalog::Column>> findColumn(const std::string& columnName) const;
   std::pair<std::string, std::vector<catalog::Column>> findColumn(const std::string& columnName, const std::string& alias) const;

   std::vector<std::pair<std::string, catalog::Column>> getColumns() const;
   std::vector<std::pair<std::string, catalog::Column>> getColumns(std::string& tableName) const;

   std::pair<std::string, std::shared_ptr<ast::FunctionInfo>> findFunction(const std::string& functionName) const;

   std::string toString() const;
};
} // namespace lingodb::analyzer