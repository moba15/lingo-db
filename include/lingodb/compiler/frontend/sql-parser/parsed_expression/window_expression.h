#pragma once
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include "lingodb/compiler/frontend/sql-parser/result_modifier.h"
namespace lingodb::ast {
enum class WindowBoundary : uint8_t {
   INVALID = 0,
   UNBOUNDED_PRECEDING = 1,
   UNBOUNDED_FOLLOWING = 2,
   CURRENT_ROW_RANGE = 3,
   CURRENT_ROW_ROWS = 4,
   EXPR_PRECEDING_ROWS = 5,
   EXPR_FOLLOWING_ROWS = 6,
   EXPR_PRECEDING_RANGE = 7,
   EXPR_FOLLOWING_RANGE = 8,
   CURRENT_ROW_GROUPS = 9,
   EXPR_PRECEDING_GROUPS = 10,
   EXPR_FOLLOWING_GROUPS = 11
};

class WindowExpression : public ParsedExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::WINDOW;
   WindowExpression(ExpressionType type, std::string catalogName, std::string schemaName, std::string functionName);

   //TODO

   std::string functionName;

   std::vector<std::shared_ptr<ParsedExpression>> children;

   std::vector<std::shared_ptr<ParsedExpression>> partitions;

   std::vector<OrderByModifier> orders;

   /// The window boundaries
   WindowBoundary start = WindowBoundary::INVALID;
   WindowBoundary end = WindowBoundary::INVALID;
};
} // namespace lingodb::ast