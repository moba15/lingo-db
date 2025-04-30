#pragma once
#include "parsed_expression.h"
#include "tableref.h"

#include <memory>
namespace lingodb::ast {
enum class PipeOperatorType : uint8_t {
   EXPRESSION = 1,
   TABLE_REF = 2,
};
class PipeOperator : public AstNode {
   public:
   PipeOperator(std::shared_ptr<AstNode> node);
   PipeOperatorType type;
   std::shared_ptr<AstNode> node;
   std::shared_ptr<PipeOperator> next;

   bool isLast();

   std::string toAsciiAST(uint32_t depth) override;
   std::string toDotGraph(uint32_t depth) override;
};
} // namespace lingodb::ast