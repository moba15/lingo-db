#pragma once
#include "lingodb/compiler/frontend/sql-parser/common/column_semantic.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/targets_expression.h"
#include "lingodb/compiler/frontend/sql-parser/pipe_operator.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
#include "lingodb/compiler/frontend/sql-parser/tableref.h"

#include <memory>
#include <variant>
#include <vector>
namespace lingodb::ast {
using pipeOp = std::variant<std::shared_ptr<QueryNode>, std::shared_ptr<TableRef>, std::shared_ptr<ParsedExpression>, std::shared_ptr<TargetsExpression>>;
class PipeSelectNode : public QueryNode {
   public:
   static constexpr QueryNodeType TYPE = QueryNodeType::PIPE_NODE;
   PipeSelectNode();
   ~PipeSelectNode() override;
   std::shared_ptr<PipeOperator> startPipeOperator;
   std::shared_ptr<PipeOperator> endPipeOperator;

   /*
    *Semantic
    */
   TargetInfo targetInfo{};

   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};
} // namespace lingodb::ast