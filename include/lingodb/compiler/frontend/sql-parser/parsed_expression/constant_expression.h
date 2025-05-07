#pragma once
#include "lingodb/compiler/frontend/sql-parser/common/constant_value.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"

#include <string>
#include <vector>
namespace lingodb::ast {
class ConstantExpression : public ParsedExpression {
   public:
   static constexpr ExpressionClass TYPE = ExpressionClass::CONSTANT;
   ConstantExpression();

   std::shared_ptr<ConstantValue> value;

   std::string toAsciiAST(uint32_t depth) override;
   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
} // namespace lingodb::ast
