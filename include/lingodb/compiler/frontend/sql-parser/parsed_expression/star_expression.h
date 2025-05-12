#pragma once
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"

#include <memory>
#include <string>
#include <vector>
namespace lingodb::ast {
class StarExpression : public ParsedExpression {
   public:
   static constexpr const ExpressionClass TYPE = ExpressionClass::STAR;

   explicit StarExpression(std::string relationName);
   //! The relation name in case of tbl.*, or empty if this is a normal *
   std::string relationName;

   //TODO add missing variables

   //! The expression to select the columns (regular expression or list)
   std::shared_ptr<ParsedExpression> expr;

   //! Whether or not this is a COLUMNS expression
   bool columnsExpr = false;

   /*
    * Semantic
    */
   //Columns and their scope
   std::vector<std::pair<std::string, catalog::Column>> columns{};

   std::string toAsciiAST(uint32_t depth) override;
   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
} // namespace lingodb::ast