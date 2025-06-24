#pragma once

#include "lingodb/compiler/frontend/sql_scope.h"
#include "lingodb/compiler/frontend/ast/aggregation_node.h"
#include "lingodb/compiler/frontend/ast/extend_node.h"

#include <llvm/ADT/ScopedHashTable.h>

#include <memory>
#include <vector>
#include <stack>
namespace lingodb::analyzer {
struct StringInfo {
   static bool isEqual(std::string a, std::string b) { return a == b; }
   static std::string getEmptyKey() { return ""; }
   static std::string getTombstoneKey() { return "-"; }
   static size_t getHashValue(std::string str) { return std::hash<std::string>{}(str); }
};

class ASTTransformContext {
   public:
   ASTTransformContext();
   std::shared_ptr<ast::AggregationNode> aggregationNode;
   std::shared_ptr<ast::ExtendNode> extendNode;
};
class SQLContext;
class DefineScope {
   public:
   DefineScope(SQLContext& sqlContext);
   ~DefineScope();

   private:

   SQLContext& sqlContext;

};

class SQLContext {
   public:
   SQLContext() : definedAttributes(), resolver() {
      definedAttributes.push({});
   };
   std::shared_ptr<catalog::Catalog> catalog;
   std::vector<std::shared_ptr<SQLScope>> scopes;
   std::shared_ptr<SQLScope> currentScope;

   std::stack<std::vector<std::pair<std::string, std::shared_ptr<ast::NamedResult>>>> definedAttributes;

   llvm::ScopedHashTable<std::string, std::shared_ptr<ast::NamedResult>, StringInfo> resolver;
   using ResolverScope = llvm::ScopedHashTable<std::string, std::shared_ptr<ast::NamedResult>, StringInfo>::ScopeTy;

   std::unordered_map<std::string, size_t> scopeUnifier;

   void pushNewScope();
   void pushNewScope(std::shared_ptr<SQLScope> newScope);
   void popCurrentScope();

   ResolverScope createResolverScope();

   DefineScope createDefineScope();

   std::vector<std::pair<std::string, std::shared_ptr<ast::NamedResult>>> getTopDefinedColumns();

   void mapAttribute(ResolverScope& scope, std::string name, std::shared_ptr<ast::NamedResult> columnInfo);
   void mapAttribute(ResolverScope& scope, std::string name, std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry);
   std::shared_ptr<ast::NamedResult> getNamedResultInfo(location loc,std::string name);

   std::string getUniqueScope(std::string base);

   //std::vector<std::pair<size_t, ast::ColumnInfo>> findColumn(const std::string& columnName) const;
   //std::vector<std::pair<size_t, ast::ColumnInfo>> findColumn(const std::string& columnName, const std::string& alias) const;


   std::string toString() const;
};
} // namespace lingodb::analyzer