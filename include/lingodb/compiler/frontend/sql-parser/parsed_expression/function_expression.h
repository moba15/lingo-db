#pragma once
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"

#include <memory>
#include <vector>

namespace lingodb::ast {
class FunctionExpression : public ParsedExpression {
   public:
   static constexpr ExpressionClass TYPE = ExpressionClass::FUNCTION;
   //TODO Finish constructor
   FunctionExpression(std::string catalog, std::string schema, std::string functionName, bool isOperator, bool distinct, bool exportState);

   //! Catalog of the function
   std::string catalog;
   //! Schema of the function
   std::string schema;
   //! Function name
   std::string functionName;
   //! Whether or not the function is an operator, only used for rendering
   bool isOperator;
   //! List of arguments to the function
   std::vector<std::shared_ptr<ParsedExpression>> arguments;
   //! Whether or not the aggregate function is distinct, only used for aggregates
   bool distinct;
   //! Expression representing a filter, only used for aggregates
   std::shared_ptr<ParsedExpression> filter;
   //! Modifier representing an ORDER BY, only used for aggregates
   std::shared_ptr<ParsedExpression> orderBy;
   //! whether this function should export its state or not
   bool exportState;

   /*
    * Semanatic
    */
   std::string scope;

   std::string toAsciiAST(uint32_t depth) override;
   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

static std::vector<std::string> aggregationFunctions{
   "min",
   "max",
   "avg"};
} // namespace lingodb::ast