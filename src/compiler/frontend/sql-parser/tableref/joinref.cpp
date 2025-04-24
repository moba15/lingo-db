#include "lingodb/compiler/frontend/sql-parser/tableref/joinref.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
JoinRef::JoinRef(JoinType type, JoinCondType refType): TableRef(TYPE), type(type), refType(refType) {
}
std::string JoinRef::toAsciiAST(uint32_t depth){
    toAsciiASTPrefix
    ast.append("JoinRef $1: left  $2: right $3: condition/using \n");
    if (left) {
       ast.append(left->toAsciiAST(depth + 1));
    }

    if (right) {
       ast.append(right->toAsciiAST(depth + 1));
    }

   if (std::holds_alternative<std::shared_ptr<ParsedExpression>>(condition)) {
      auto cond = std::get<std::shared_ptr<ParsedExpression>>(condition);
      if (cond)
         ast.append(cond->toAsciiAST(depth + 1));
   } else if (std::holds_alternative<std::vector<std::shared_ptr<ColumnRefExpression>>>(condition)) {
      auto columnRefs = std::get<std::vector<std::shared_ptr<ColumnRefExpression>>>(condition);
      for (auto& columnRef : columnRefs) {
         ast.append(columnRef->toAsciiAST(depth + 1));
      }

   }







    return ast;
}
} // namespace lingodb::ast