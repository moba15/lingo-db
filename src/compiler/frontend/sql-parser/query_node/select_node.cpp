#include "lingodb/compiler/frontend/sql-parser/query_node/select_node.h"

#include <iostream>
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
      if (!select) {
         std::cerr << "Select List seems empty" << std::endl;
         continue;

      }

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

   if (groups) {
      ast.append(std::string(depth+1,'\t'));
      ast.append("└── groups\n");
      ast.append(groups->toAsciiAST(depth+2));
   }

   if (having) {
      ast.append(std::string(depth+1,'\t'));
      ast.append("└── having\n");
      ast.append(having->toAsciiAST(depth+2));
   }

   return ast;
}


} // namespace lingodb::ast