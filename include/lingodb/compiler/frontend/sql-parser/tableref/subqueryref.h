#pragma once
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/columnref_expression.h"
#include "lingodb/compiler/frontend/sql-parser/pipe_operator.h"
#include "lingodb/compiler/frontend/sql-parser/query_node/pipe_select_node.h"
#include "lingodb/compiler/frontend/sql-parser/query_node/select_node.h"
#include "lingodb/compiler/frontend/sql-parser/tableref.h"

#include <memory>
#include <variant>
#include <vector>
namespace lingodb::ast {
enum class JoinCondType : uint8_t;
enum class JoinType : uint8_t;
using jointCondOrUsingCols = std::variant<std::shared_ptr<ParsedExpression>, std::vector<std::shared_ptr<ColumnRefExpression>>>;
class SubqueryRef : public TableRef {
   static constexpr TableReferenceType TYPE = TableReferenceType::SUBQUERY;

   public:
   SubqueryRef(std::shared_ptr<QueryNode> subSelectNode);

   //! The subquery
   //!TODO correct Type
   std::shared_ptr<QueryNode> subSelectNode;


   std::string toDotGraph(uint32_t depth, NodeIdGenerator& idGen) override;
};

} // namespace lingodb::ast
