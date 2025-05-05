#pragma once
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"

#include <memory>
#include <vector>

namespace lingodb::ast {
class ConjunctionExpression : public ParsedExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::CONJUNCTION;
   explicit ConjunctionExpression(ExpressionType type);
   ConjunctionExpression(ExpressionType type, std::shared_ptr<lingodb::ast::ParsedExpression> left, std::shared_ptr<lingodb::ast::ParsedExpression> right);
   ConjunctionExpression(ExpressionType type, std::vector<std::shared_ptr<ParsedExpression>> children);

   std::vector<std::shared_ptr<ParsedExpression>> children;

   std::string toAsciiAST(uint32_t depth) override;
   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;

   private:
   std::string typeToAscii(ExpressionType type) const;
};
} // namespace lingodb::ast