#pragma once
#include "ast_node.h"
///A node that produces a relation/table
/// Takes a Table as input and produces a relation/table as output
namespace lingodb::ast {
class TableProducer : public AstNode {
   public:
   TableProducer(NodeType type);




};
} // namespace lingodb::ast