#pragma once
#include "parsed_expression.h"
#include "tabel_producer.h"
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
class PipeOperator : public TableProducer {
   public:
   PipeOperator(PipeOperatorType pipeOpType, std::shared_ptr<AstNode> node);
   PipeOperatorType pipeOpType;
   std::shared_ptr<AstNode> node;

   bool isLast();

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
} // namespace lingodb::ast