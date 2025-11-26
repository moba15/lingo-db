#ifndef LINGODB_COMPILER_FRONTEND_SQL_CONTEXT_H
#define LINGODB_COMPILER_FRONTEND_SQL_CONTEXT_H

#include "lingodb/compiler/frontend/ast/aggregation_node.h"
#include "lingodb/compiler/frontend/ast/bound/bound_query_node.h"
#include "lingodb/compiler/frontend/ast/extend_node.h"
#include "lingodb/compiler/frontend/sql_scope.h"

#include <llvm/ADT/ScopedHashTable.h>

#include <memory>
#include <stack>
#include <unordered_set>
#include <vector>
namespace lingodb::ast {
class CTENode;
} // namespace lingodb::ast
namespace lingodb::analyzer {

struct StringInfo {
   static bool isEqual(std::string a, std::string b) { return a == b; }
   static std::string getEmptyKey() { return ""; }
   static std::string getTombstoneKey() { return "-"; }
   static size_t getHashValue(std::string str) { return std::hash<std::string>{}(str); }
};
/**
 * @ASTTransformScope and @ASTTransformContext are used to store useful information that is needed by the Canonicalizer.
 */
class ASTTransformScope {
   public:
   ASTTransformScope() : aggregationNode(std::make_shared<ast::AggregationNode>()), extendNodeBeforeWindowFunctions(std::make_shared<ast::ExtendNode>(true)) {}
   std::shared_ptr<ast::AggregationNode> aggregationNode;
   std::shared_ptr<ast::ExtendNode> extendNodeBeforeWindowFunctions;
   std::unordered_map<std::shared_ptr<ast::ParsedExpression>, std::shared_ptr<ast::ColumnRefExpression>, ast::ParsedExprPtrHash, ast::ParsedExprPtrEqual> alreadyExtendedExpression;
   /**
    * Maps aliases from the SELECT list to their underlying expressions.
    * Used when resolving GROUP BY clauses that reference SELECT aliases.
    * Example: SELECT z AS y FROM ... GROUP BY y
    *          → 'y' in the GROUP BY must be resolved to expression 'z'.
    */
   std::unordered_map<std::string, std::shared_ptr<ast::ParsedExpression>> selectSymbolList;
   /// Container for GROUP BY expressions that ensures uniqueness, reuse in the canonicalization step
   std::unordered_set<std::shared_ptr<ast::ParsedExpression>, ast::ParsedExprPtrHash, ast::ParsedExprPtrEqual> groupedByExpressions;

   bool classicalSelect = false;
};
class ASTTransformContext {
   public:
   ASTTransformContext();
   std::stack<std::shared_ptr<ASTTransformScope>> scopeStack{};
   std::shared_ptr<ASTTransformScope> currentScope;
   void pushNewScope() {
      currentScope = std::make_shared<ASTTransformScope>();
      scopeStack.push(currentScope);
   }

   void popScope() {
      currentScope = scopeStack.top();
      scopeStack.pop();
   }
};
class SQLContext;
/**
 * A DefineScope is used to control the scope of SQLContext::definedAttributes
*/
class DefineScope {
   public:
   DefineScope(SQLContext& sqlContext);
   ~DefineScope();

   private:
   SQLContext& sqlContext;
};
/**
 * Stores the colleted context during analysis
 * SQLContext can be used to
 *    - store/lookup Columns inside a specific scope
 *    - store/lookup CTE nodes
 */
class SQLContext {
   using ResolverImpl = llvm::ScopedHashTable<std::string, std::vector<std::shared_ptr<ast::ColumnReference>>, StringInfo>;
   public:
   SQLContext() : definedAttributes(), resolver() {
      definedAttributes.push({});
   };

   catalog::Catalog* catalog;
   std::vector<std::shared_ptr<SQLScope>> scopes;
   std::shared_ptr<SQLScope> currentScope;
   bool ignore = false;

   std::stack<std::vector<std::pair<std::string, std::shared_ptr<ast::ColumnReference>>>> definedAttributes;
   using renameCte = std::vector<std::pair<std::shared_ptr<ast::ColumnReference>, std::shared_ptr<ast::ColumnReference>>>;
   std::unordered_map<std::string, std::pair<ast::TargetInfo, std::shared_ptr<ast::BoundCTENode>>> ctes;

   std::unordered_map<std::string, mlir::Value> translatedCtes;
   struct Cmp {
      bool operator()(const std::shared_ptr<ast::ColumnReference>& a, const std::shared_ptr<ast::ColumnReference>& b) const {
         if (!a || !b) {
            return !b; // Handle null pointers consistently
         }
         // Assumes 'scope' and 'name' uniquely identify a column reference
         return std::tie(a->scope, a->name) < std::tie(b->scope, b->name);
      }
   };
   llvm::ScopedHashTable<std::string, std::set<std::shared_ptr<ast::ColumnReference>, Cmp>, StringInfo> resolver;
   class ResolverScope : public llvm::ScopedHashTableScope<std::string, std::vector<std::shared_ptr<ast::ColumnReference>>, StringInfo> {
      public:
      explicit ResolverScope(ResolverImpl& HT)
          : llvm::ScopedHashTableScope<std::string, std::vector<std::shared_ptr<ast::ColumnReference>>, StringInfo>(HT) {}
      ResolverImpl* getHT() { return this->HT; }
   };
   std::unordered_map<std::string, size_t> scopeUnifier;

   void pushNewScope();
   void pushNewScope(std::shared_ptr<SQLScope> newScope);
   void popCurrentScope();

   ResolverScope createResolverScope();

   DefineScope createDefineScope();

   std::vector<std::pair<std::string,std::shared_ptr<ast::ColumnReference>>> getTopDefinedColumns();

   void mapAttribute(ResolverScope& scope, std::string name, std::shared_ptr<ast::ColumnReference> columnInfo);
   std::vector<std::shared_ptr<ast::ColumnReference>> mapAttribute(ResolverScope& scope, std::string sqlScopeName, std::string uniqueScope, std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry);
   void mapAttribute(ResolverScope& scope, std::string name, std::vector<std::shared_ptr<ast::ColumnReference>> targetInfos);
   std::shared_ptr<ast::ColumnReference> getColumnReference(location loc, std::string name);
   void replace(ResolverScope& scope, std::shared_ptr<ast::ColumnReference> old, std::shared_ptr<ast::ColumnReference> value);

   void replace2(ResolverScope& scope, std::string key, std::shared_ptr<ast::ColumnReference> old, std::shared_ptr<ast::ColumnReference> value) {
      std::vector<std::pair<std::string, std::shared_ptr<ast::ColumnReference>>> toReplace;
      std::ranges::copy_if(definedAttributes.top(), std::back_inserter(toReplace), [&](auto& p) { return p.second == old; });
      for (auto& c : toReplace) {
         mapAttribute(scope, c.first, value);
      }
      auto x = resolver.lookup(key);

      if (!x.empty()) {
         std::set<std::shared_ptr<ast::ColumnReference>, Cmp> y{};
         for (auto& c : x) {
            if (c == old) {
               y.insert(value);
            } else {
               y.insert(c);
            }
         }
         resolver.insertIntoScope(&scope, key, {y});
      }
   }

   std::string getUniqueScope(std::string base);
};
} // namespace lingodb::analyzer
#endif
