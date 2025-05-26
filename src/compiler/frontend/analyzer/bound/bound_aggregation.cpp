#include "lingodb/compiler/frontend/analyzer/bound/bound_aggregation.h"
namespace lingodb::ast {
BoundAggregationNode::BoundAggregationNode(std::shared_ptr<BoundGroupByNode> groupByNode, std::vector<std::shared_ptr<BoundFunctionExpression>> aggregations) : AstNode(NodeType::BOUND_AGGREGATION), groupByNode(groupByNode), aggregations(aggregations) {
}
std::string BoundAggregationNode::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   return "";
}
} // namespace lingodb::ast