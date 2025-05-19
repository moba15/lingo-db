#pragma once
#include "parsed_expression.h"
#include "tableref.h"

#include <memory>
namespace lingodb::ast {
enum class PipeOperatorType : uint8_t {
   WHERE = 1,
   SELECT = 2,
   JOIN = 3,
   GROUP_BY = 4,
   ORDER_BY = 5,
   LIMIT = 6,
   UNION = 7,
   INTERSECT = 8,
   EXCEPT = 9,
   FROM = 10,
   AGGREGATE = 11,

};
class PipeOperator : public AstNode {
   public:
   PipeOperator(PipeOperatorType type, std::shared_ptr<AstNode> node);
   PipeOperatorType type;
   std::shared_ptr<AstNode> node;
   std::shared_ptr<PipeOperator> next;

   bool isLast();


   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
} // namespace lingodb::ast