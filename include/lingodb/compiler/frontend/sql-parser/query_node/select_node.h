#pragma once
#include "lingodb/compiler/frontend/sql-parser/group_by_node.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression.h"
#include "lingodb/compiler/frontend/sql-parser/parsed_expression/targets_expression.h"
#include "lingodb/compiler/frontend/sql-parser/query_node.h"
#include "lingodb/compiler/frontend/sql-parser/tableref.h"

#include <memory>
#include <vector>
namespace lingodb::ast {
class SelectNode : public QueryNode {
public:
  SelectNode();
   ~SelectNode() override;
  static constexpr QueryNodeType TYPE = QueryNodeType::SELECT_NODE;
  //! The projection list
  std::shared_ptr<TargetsExpression> select_list;
  //! The FROM clause
  std::shared_ptr<TableRef> from_clause;
  //! The WHERE clause
  std::shared_ptr<ParsedExpression> where_clause;

   //! list of groups
   std::shared_ptr<GroupByNode> groups;

   //! HAVING clause
   std::shared_ptr<ParsedExpression> having;

  //TODO add missing parameters
   std::string toString(uint32_t depth) override;

   std::string toAsciiAST(uint32_t depth) override;
};
} // namespace lingodb::ast
