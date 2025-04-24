#include "lingodb/compiler/frontend/sql-parser/query_node/select_node.h"
namespace lingodb::ast {
SelectNode::SelectNode(): QueryNode(TYPE) {

}
std::string SelectNode::toString(uint32_t depth) {
   return "SelectNode";
};
SelectNode::~SelectNode()  = default;

std::string SelectNode::toAsciiAST(uint32_t depth ) {
   std::string ast{};
   ast.append(std::string(depth,'\t' ));
   ast.append("└──");
   ast.append("SelectNode");
   ast.append("\n");
   ast.append(std::string(depth+1,'\t'));
   ast.append("└── selectList\n");
   for (auto select: select_list) {
      ast.append(select->toAsciiAST(depth+2));
   }
   if (where_clause) {
      ast.append(std::string(depth+1,'\t'));
      ast.append("└── whereClause\n");

      ast.append(where_clause->toAsciiAST(depth+2));
   }

   if (from_clause) {
      ast.append(std::string(depth+1,'\t'));
      ast.append("└── from_clause\n");

      ast.append(from_clause->toAsciiAST(depth+2));
   }


   return ast;
}


} // namespace lingodb::ast