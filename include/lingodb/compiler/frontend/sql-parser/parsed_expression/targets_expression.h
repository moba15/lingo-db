#pragma once
#include "lingodb/catalog/Column.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include <memory>
#include <vector>
namespace lingodb::ast {
//List of targets
//Used for the select_list
//Select ...,...,...
class TargetsExpression : public ParsedExpression {
   public:
   static constexpr ExpressionClass TYPE = ExpressionClass::TARGETS;
   TargetsExpression();

   std::vector<std::shared_ptr<ParsedExpression>> targets{};

   /*
    * Semantic
    */
   //TODO make std::vector<mlir::Attribute> names; instead of std::string vector
   std::vector<std::string> names{};
   //TODO only reference to column
   std::vector<std::pair<std::string, catalog::Column>> targetColumns{};

   std::string toAsciiAST(uint32_t depth) override;
   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
} // namespace lingodb::ast