#pragma once
#include "lingodb/compiler/frontend/sql-parser/driver.h"
#include "lingodb/compiler/frontend/sql-parser/gen/location.hh"
#include "sql_context.h"

#include <memory>

namespace lingodb::analyzer {
class Analyzer {
   public:
   Analyzer() = default;
   virtual ~Analyzer() = default;

   void virtual analyze(std::shared_ptr<ast::AstNode> rootNode, std::shared_ptr<SQLContext> context) = 0;

   virtual void error(std::string message, lingodb::location loc) {

   };
   driver drv{};
};
} // namespace lingodb::analyzer