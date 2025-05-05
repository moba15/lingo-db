#pragma once
#include "base_expression.h"

#include <string>
namespace lingodb::ast {
class ParsedExpression : public BaseExpression {
   public:
   ParsedExpression(ExpressionType type, ExpressionClass expression_class) : BaseExpression(type, expression_class) {
   }

   virtual std::string toAsciiAST(uint32_t depth) = 0;
   virtual std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) = 0;
};
} // namespace lingodb::ast
