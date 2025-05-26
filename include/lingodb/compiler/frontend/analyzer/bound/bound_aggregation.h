#pragma once
#include "bound_expression.h"
#include "bound_groupby.h"
#include "lingodb/compiler/frontend/sql-parser/common/node_id_generator.h"
namespace lingodb::ast {
class BoundAggregationNode : public AstNode {
   public:
   BoundAggregationNode(std::shared_ptr<BoundGroupByNode> groupByNode, std::vector<std::shared_ptr<BoundFunctionExpression>> aggregations);

   std::shared_ptr<BoundGroupByNode> groupByNode;
   std::vector<std::shared_ptr<BoundFunctionExpression>> aggregations;
   //TODO having clause

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
}