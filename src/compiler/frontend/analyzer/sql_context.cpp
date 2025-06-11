#include "lingodb/compiler/frontend/analyzer/sql_context.h"

#include "lingodb/runtime/RecordBatchInfo.h"

#include <iostream>
namespace lingodb::analyzer {
using ResolverScope = llvm::ScopedHashTable<std::string, std::shared_ptr<ast::NamedResult>, StringInfo>::ScopeTy;
ASTTransformContext::ASTTransformContext() : aggregationNode(std::make_shared<ast::AggregationNode>()), extendNode(std::make_shared<ast::ExtendNode>()){
}

DefineScope::DefineScope(SQLContext& sqlContext) : sqlContext(sqlContext) {
   sqlContext.definedAttributes.push({});
}
DefineScope::~DefineScope() {
   sqlContext.definedAttributes.pop();
}


void SQLContext::pushNewScope() {
   auto newScope = std::make_shared<SQLScope>();
   newScope->parent = currentScope;
   currentScope = newScope;
   scopes.push_back(newScope);
}

void SQLContext::pushNewScope(std::shared_ptr<SQLScope> newScope) {
   newScope->parent = currentScope;
   currentScope = newScope;
   scopes.push_back(newScope);
}
void SQLContext::popCurrentScope() {
   if (!currentScope->parent) {
      std::cerr << "Cannot pop root scope" << std::endl;
      return;
   }
   currentScope = currentScope->parent;
   scopes.pop_back();
}
ResolverScope SQLContext::createResolverScope() {
   return ResolverScope(resolver);
}

DefineScope SQLContext::createDefineScope() {
   return DefineScope(*this);
}

std::vector<std::pair<std::string, std::shared_ptr<ast::NamedResult>>> SQLContext::getTopDefinedColumns() {
   return definedAttributes.top();
}
void SQLContext::mapAttribute(ResolverScope& scope, std::string name, std::shared_ptr<ast::NamedResult> columnInfo) {
   definedAttributes.top().push_back({name, columnInfo});
   resolver.insertIntoScope(&scope, name, columnInfo);
}

void SQLContext::mapAttribute(ResolverScope& scope, std::string name, std::shared_ptr<catalog::TableCatalogEntry> tableCatalogEntry) {
   for (auto c : tableCatalogEntry->getColumns()) {
      auto columnInfo = std::make_shared<ast::ColumnInfo>(name, c);

      mapAttribute(scope, name + "." + c.getColumnName(), columnInfo);

      mapAttribute(scope, c.getColumnName(), columnInfo);


   }

}
std::shared_ptr<ast::NamedResult> SQLContext::getNamedResultInfo(std::string name) {
   if (!resolver.count(name)) {
      throw std::runtime_error("could not resolve '" + name + "'");
   }
   const auto res = resolver.lookup(name);
   return res;
}


/*std::vector<std::pair<size_t, ast::ColumnInfo>> SQLContext::findColumn(const std::string& columnName) const {
   std::vector<std::pair<size_t, ast::ColumnInfo>> columns{};
   std::string foundTable;
   auto current = currentScope;
   while (current) {
      for (auto t : current->columns) {
         auto colsOfTable = t.second;
         foundTable = t.first;
         auto it = std::find_if(colsOfTable.begin(), colsOfTable.end(), [&columnName](std::pair<size_t, ast::ColumnInfo> col) {
            const std::string& colName = col.second.column.getColumnName();
            return colName.length() == columnName.length() &&
               std::equal(colName.begin(), colName.end(), columnName.begin(),
                          [](char a, char b) { return std::tolower(a) == std::tolower(b); });
         });
         if (it != colsOfTable.end()) {
            columns.emplace_back(*it);
         }
      }
      current = current->parent;
   }
   return columns;
}*/



std::string SQLContext::toString() const {
   return "";
}

} // namespace lingodb::analyzer