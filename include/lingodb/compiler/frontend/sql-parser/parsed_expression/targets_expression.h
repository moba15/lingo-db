#pragma once
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include <vector>
#include <memory>
namespace lingodb::ast {
//List of targets
//Used for the select_list
//Select ...,...,...
class TargetsExpression : public ParsedExpression {
   public:
   static constexpr ExpressionClass TYPE = ExpressionClass::TARGETS;
   TargetsExpression();

   std::vector<std::shared_ptr<ParsedExpression>> targets{};
   std::string toAsciiAST(uint32_t depth ) override;
   std::string toDotGraph(uint32_t depth) override;
};
} // namespace lingodb::ast