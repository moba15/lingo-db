#pragma once
#include "lingodb/compiler/frontend/analyzer/sql_scope.h"
#include "lingodb/compiler/frontend/sql-parser/aggregation_node.h"

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
   std::vector<std::pair<size_t, ast::ColumnInfo>> findColumn(const std::string& columnName) const;
   std::vector<std::pair<size_t, ast::ColumnInfo>> findColumn(const std::string& columnName, const std::string& alias) const;
   std::vector<std::pair<size_t, ast::ColumnInfo>> findColumn(const size_t id) const;

   std::vector<std::pair<std::string, catalog::Column>> getColumns() const;
   std::vector<std::pair<std::string, catalog::Column>> getColumns(std::string& tableName) const;

   std::optional<std::pair<size_t, ast::FunctionInfo>> findFunction(const std::string& functionName) const;
   std::optional<std::pair<size_t, ast::FunctionInfo>> findFunction(const size_t id) const;

   std::string toString() const;
};
} // namespace lingodb::analyzer