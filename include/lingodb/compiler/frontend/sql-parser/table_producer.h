#pragma once
#include "ast_node.h"
///A node that produces a relation/table
/// Takes a Table as input and produces a relation/table as output
namespace lingodb::ast {
class TableProducer : public AstNode {
   public:
   TableProducer(NodeType type);

   std::shared_ptr<TableProducer> input = nullptr;

   virtual std::string inputToDotGraph(uint32_t depth, NodeIdGenerator& idGen);
};
} // namespace lingodb::ast