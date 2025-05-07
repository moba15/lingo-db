#include "lingodb/compiler/frontend/analyzer/sql_pass/resolve_name_pass.h"

#include "lingodb/compiler/frontend/sql-parser/tableref/joinref.h"

#include <lingodb/compiler/frontend/sql-parser/query_node/select_node.h>
#include <lingodb/compiler/frontend/sql-parser/tableref/basetableref.h>
namespace lingodb::analyzer {
void ResolveNamePass::analyze(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<SQLContext> context) {
   /* if (rootNode->type == ast::QueryNodeType::SELECT_NODE) {
      //Create new scope for this select Query
      context->pushNewScope();
      auto selectNode = std::static_pointer_cast<ast::SelectNode>(rootNode);
      auto from = selectNode->from_clause;
      if (from->type == ast::TableReferenceType::BASE_TABLE) {
         auto baseTable = std::static_pointer_cast<ast::BaseTableRef>(from);
         //TODO find correct table form catalogs
         auto catalogEntryOpt = context->catalog->getTypedEntry<lingodb::catalog::TableCatalogEntry>(baseTable->tableName);
         if (catalogEntryOpt.has_value()) {
            auto name = baseTable->tableName;
            if (!baseTable->alias.empty()) {
               name = baseTable->alias;
            }
            context->currentScope->tables.emplace(std::pair{name,catalogEntryOpt.value()});
         } else {
            //TODO throw error
            std::cerr << "Table \"" << baseTable->tableName << "\" not found: " << baseTable->loc << std::endl;
         }

      } else if (from->type == ast::TableReferenceType::JOIN) {
         auto join = std::static_pointer_cast<ast::JoinRef>(from);
         analyze(std::dynamic_pointer_cast<ast::QueryNode>(join->left), context);
         analyze(std::dynamic_pointer_cast<ast::QueryNode>(join->right), context);
         if (std::holds_alternative<std::shared_ptr<ast::ParsedExpression>>(join->condition)) {
            analyze(std::get<std::shared_ptr<ast::ParsedExpression>>(join->condition),context);
         }
         analyze(std::dynamic_pointer_cast<ast::QueryNode>(join->condition), context);
      }
   }*/
}
} // namespace lingodb::analyzer