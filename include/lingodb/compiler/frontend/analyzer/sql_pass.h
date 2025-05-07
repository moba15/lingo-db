#pragma once
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
#include "sql_context.h"

#include <memory>
namespace lingodb::analyzer {
class SQLPass {
   public:
   virtual ~SQLPass() = default;

   private:
   virtual void analyze(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<SQLContext> context) = 0;
   virtual void analyze(std::shared_ptr<ast::ParsedExpression> rootExpression, std::shared_ptr<SQLContext> context) = 0;
   void analyze(std::vector<std::shared_ptr<ast::ParsedExpression>> rootExpressions, std::shared_ptr<SQLContext> context);
};
} // namespace lingodb::analyzer
