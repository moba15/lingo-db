#ifndef LINGODB_PL_EXPRESSION_H
#define LINGODB_PL_EXPRESSION_H
#include <memory>
#include <lingodb/compiler/frontend/ast/ast_node.h>
#include <lingodb/compiler/frontend/ast/table_producer.h>

namespace lingodb::ast {
enum class PlExpressionType : uint8_t {
   RETURN
};
class PlExpression : public AstNode {
   public:
   PlExpression(PlExpressionType exprType) : AstNode(NodeType::PL_EXPRESSION), exprType(exprType) {}

   PlExpressionType exprType;
};

class ReturnPlExpression : public PlExpression {
   public:
   ReturnPlExpression(std::shared_ptr<TableProducer> producer) : PlExpression(PlExpressionType::RETURN), producer(producer) {}
   std::shared_ptr<TableProducer> producer;
};
} // namespace lingodb::ast

#endif //LINGODB_PL_EXPRESSION_H
