#ifndef LINGODB_PL_EXPRESSION_H
#define LINGODB_PL_EXPRESSION_H
#include <lingodb/compiler/frontend/ast/ast_node.h>

namespace lingodb::ast {
class PlExpression : public AstNode {
   public:
   PlExpression() : AstNode(NodeType::PL_EXPRESSION) {}
};
} // namespace lingodb::ast

#endif //LINGODB_PL_EXPRESSION_H
