#include "lingodb/compiler/frontend/analyzer/sql_pass.h"
namespace lingodb::analyzer {
void SQLPass::analyze(std::vector<std::shared_ptr<ast::ParsedExpression>> rootExpressions, std::shared_ptr<SQLContext> context) {
   for (auto& rootExpression : rootExpressions) {
      analyze(rootExpression, context);
   }
}
} // namespace lingodb::analyzer