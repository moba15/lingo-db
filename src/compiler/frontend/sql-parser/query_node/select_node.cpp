#include "lingodb/compiler/frontend/sql-parser/query_node/select_node.h"

#include <iostream>
namespace lingodb::ast {
SelectNode::SelectNode(): QueryNode(TYPE) {

}
std::string SelectNode::toString(uint32_t depth) {
   return "SelectNode";
};
SelectNode::~SelectNode()  = default;

std::string SelectNode::toAsciiAST(uint32_t depth) {
   std::string ast{};
   ast.append(std::string(depth, '\t'));
   ast.append("└──");
   ast.append("SelectNode");
   ast.append("\n");

   if (select_list) {
      ast.append(std::string(depth + 1, '\t'));
      ast.append("└── selectList\n");
      ast.append(select_list->toAsciiAST(depth + 2));
   }

   if (where_clause) {
      ast.append(std::string(depth + 1, '\t'));
      ast.append("└── whereClause\n");

      ast.append(where_clause->toAsciiAST(depth + 2));
   }

   if (from_clause) {
      ast.append(std::string(depth + 1, '\t'));
      ast.append("└── from_clause\n");

      ast.append(from_clause->toAsciiAST(depth + 2));
   }

   if (groups) {
      ast.append(std::string(depth + 1, '\t'));
      ast.append("└── groups\n");
      ast.append(groups->toAsciiAST(depth + 2));
   }

   if (having) {
      ast.append(std::string(depth + 1, '\t'));
      ast.append("└── having\n");
      ast.append(having->toAsciiAST(depth + 2));
   }

   return ast;
}
std::string SelectNode::toDotGraph(uint32_t depth) {
    std::string dot{};
    
    // Create node identifier for the SELECT node
    std::string nodeId = "node" + std::to_string(reinterpret_cast<uintptr_t>(this));
    dot += nodeId + " [label=\"SelectNode\"];\n";
    
    // Handle select list
    if (select_list) {
        std::string selectListId = "node" + std::to_string(reinterpret_cast<uintptr_t>(select_list.get()));
        dot += nodeId + " -> " + selectListId + " [label=\"select list\"];\n";
        dot += select_list->toDotGraph(depth + 1);
    }
    
    // Handle FROM clause
    if (from_clause) {
        std::string fromId = "node" + std::to_string(reinterpret_cast<uintptr_t>(from_clause.get()));
        dot += nodeId + " -> " + fromId + " [label=\"FROM\"];\n";
        dot += from_clause->toDotGraph(depth + 1);
    }
    
    // Handle WHERE clause
    if (where_clause) {
        std::string whereId = "node" + std::to_string(reinterpret_cast<uintptr_t>(where_clause.get()));
        dot += nodeId + " -> " + whereId + " [label=\"WHERE\"];\n";
        dot += where_clause->toDotGraph(depth + 1);
    }
    
    // Handle GROUP BY
    if (groups) {
        std::string groupsId = "node" + std::to_string(reinterpret_cast<uintptr_t>(groups.get()));
        dot += nodeId + " -> " + groupsId + " [label=\"GROUP BY\"];\n";
        dot += groups->toDotGraph(depth + 1);
    }
    
    // Handle HAVING clause
    if (having) {
        std::string havingId = "node" + std::to_string(reinterpret_cast<uintptr_t>(having.get()));
        dot += nodeId + " -> " + havingId + " [label=\"HAVING\"];\n";
        dot += having->toDotGraph(depth + 1);
    }

   for (const auto& modifier : modifiers) {
      if (modifier) {
         std::string modifierId;
         modifierId.append("node");
         modifierId.append(std::to_string(reinterpret_cast<uintptr_t>(modifier.get())));

         // Add edge from select node to modifier
         dot.append(nodeId);
         dot.append(" -> ");
         dot.append(modifierId);

         // Add appropriate label based on modifier type
         dot.append(" [label=\"");
         switch (modifier->modifierType) {
            case ResultModifierType::ORDER_BY:
               dot.append("ORDER BY");
               break;
            case ResultModifierType::LIMIT:
               dot.append("LIMIT");
               break;
            case ResultModifierType::OFFSET:
               dot.append("OFFSET");
               break;
         }
         dot.append("\"];\n");

         // Add the modifier's graph representation
         dot.append(modifier->toDotGraph(depth + 1));
      }
   }

    
    return dot;
}

} // namespace lingodb::ast