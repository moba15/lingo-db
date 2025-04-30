#pragma once
#include "ast_node.h"
#include "group_by_node.h"
#include "parsed_expression/function_expression.h"
namespace lingodb::ast {
class AggregationNode : public AstNode {
   public:
   AggregationNode();

   std::shared_ptr<GroupByNode> groupByNode;
   std::vector<std::shared_ptr<FunctionExpression>> aggregations;
   //TODO having clause

   std::string toAsciiAST(uint32_t depth) override;
   std::string toDotGraph(uint32_t depth) override;
};
} // namespace lingodb::ast