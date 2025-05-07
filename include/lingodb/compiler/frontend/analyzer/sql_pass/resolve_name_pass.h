#pragma once
#include "lingodb/compiler/frontend/analyzer/sql_pass.h"
namespace lingodb::analyzer {
class ResolveNamePass : public SQLPass {
   public:
   void analyze(std::shared_ptr<ast::QueryNode> rootNode, std::shared_ptr<SQLContext> context) override;
};
} // namespace lingodb::analyzer
