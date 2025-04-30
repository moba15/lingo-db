#pragma once
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include <vector>
#include <string>
namespace lingodb::ast {
class ConstantExpression : public ParsedExpression {
  public:
  static constexpr ExpressionClass TYPE = ExpressionClass::CONSTANT;
   ConstantExpression(int iVal);
   ConstantExpression(std::string  iVal);
   ConstantExpression();

   int32_t iVal;


   std::string toAsciiAST(uint32_t depth ) override;
   std::string toDotGraph(uint32_t depth) override;


};
} // namespace lingodb::ast
