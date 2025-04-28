#include "lingodb/compiler/frontend/sql-parser/pipe_operator.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
PipeOperator::PipeOperator(std::shared_ptr<ParsedExpression> expression) : type(PipeOperatorType::EXPRESSION), expression(expression), tabelRef(nullptr){
}
PipeOperator::PipeOperator(std::shared_ptr<TableRef> tabelRef) : type(PipeOperatorType::TABLE_REF), expression(nullptr), tabelRef(tabelRef) {
}
bool PipeOperator::isLast() {
   return next == nullptr;
}
std::string PipeOperator::toAsciiAST(uint32_t depth) {
   std::string ast{};
   if (expression != nullptr) {
      ast.append(expression->toAsciiAST(depth));
   } else if (tabelRef) {
      ast.append(tabelRef->toAsciiAST(depth));
   }

   if (next)
      ast.append(next->toAsciiAST(depth));
   return ast;
}
} // namespace lingodb::ast