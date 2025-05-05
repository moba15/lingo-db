#include "lingodb/compiler/frontend/sql-parser/tableref/joinref.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
JoinRef::JoinRef(JoinType type, JoinCondType refType) : TableRef(TYPE), type(type), refType(refType) {
}
std::string JoinRef::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix
      ast.append("JoinRef $1: left  $2: right $3: condition/using \n");
   if (std::holds_alternative<std::shared_ptr<TableRef>>(left)) {
      auto l = std::get<std::shared_ptr<TableRef>>(left);
      if (l)
         ast.append(l->toAsciiAST(depth + 1));
   } else if (std::holds_alternative<std::shared_ptr<QueryNode>>(left)) {
      auto queryNode = std::get<std::shared_ptr<QueryNode>>(left);
      if (queryNode)
         ast.append(queryNode->toAsciiAST(depth + 1));
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
         //ast.append(columnRef->toAsciiAST(depth + 1));
      }
   }

   return ast;
}
std::string JoinRef::toDotGraph(uint32_t depth,  NodeIdGenerator& idGen) {
   std::string dot{};

   // Create node identifier for the join
   std::string nodeId;
   nodeId.append("node");
   nodeId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))));

   // Create the join node with its label
   dot.append(nodeId);
   dot.append(" [label=\"JoinRef\ntype: ");

   // Add join type
   switch (type) {
      case JoinType::LEFT: dot.append("left"); break;
      case JoinType::RIGHT: dot.append("right"); break;
      case JoinType::INNER: dot.append("inner"); break;
      case JoinType::OUTER: dot.append("outer"); break;
      case JoinType::SEMI: dot.append("left semi"); break;
      case JoinType::ANTI: dot.append("left anti"); break;
      case JoinType::MARK: dot.append("mark"); break;
      case JoinType::SINGLE: dot.append("single"); break;
      case JoinType::RIGHT_SEMI: dot.append("right semi"); break;
      case JoinType::RIGHT_ANTI: dot.append("right anti"); break;
      default: dot.append("invalid"); break;
   }

   dot.append(" JOIN\\n");

   // Add join condition type
   switch (refType) {
      case JoinCondType::NATURAL: dot.append("natural"); break;
      case JoinCondType::CROSS: dot.append("cross"); break;
      case JoinCondType::POSITIONAL: dot.append("positional"); break;
      case JoinCondType::ASOF: dot.append("asof"); break;
      case JoinCondType::DEPENDENT: dot.append("dependent"); break;
      default: break;
   }

   dot.append("\"];\n");

   // Add left side
   if (std::holds_alternative<std::shared_ptr<TableRef>>(left)) {
      auto leftRef = std::get<std::shared_ptr<TableRef>>(left);
      if (leftRef) {
         std::string leftId;
         leftId.append("node");
         leftId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(leftRef.get()))));

         dot.append(nodeId);
         dot.append(" -> ");
         dot.append(leftId);
         dot.append(" [label=\"left\"];\n");
         dot.append(leftRef->toDotGraph(depth + 1, idGen));
      }
   } else if (std::holds_alternative<std::shared_ptr<QueryNode>>(left)) {
      auto leftQuery = std::get<std::shared_ptr<QueryNode>>(left);
      if (leftQuery) {
         std::string leftId;
         leftId.append("node");
         leftId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(leftQuery.get()))));

         dot.append(nodeId);
         dot.append(" -> ");
         dot.append(leftId);
         dot.append(" [label=\"left\"];\n");
         dot.append(leftQuery->toDotGraph(depth + 1, idGen));
      }
   }

   // Add right side
   if (right) {
      std::string rightId;
      rightId.append("node");
      rightId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(right.get()))));

      dot.append(nodeId);
      dot.append(" -> ");
      dot.append(rightId);
      dot.append(" [label=\"right\"];\n");
      dot.append(right->toDotGraph(depth + 1, idGen));
   }

   // Add condition
   if (std::holds_alternative<std::shared_ptr<ParsedExpression>>(condition)) {
      auto condExpr = std::get<std::shared_ptr<ParsedExpression>>(condition);
      if (condExpr) {
         std::string condId;
         condId.append("node");
         condId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(condExpr.get()))));

         dot.append(nodeId);
         dot.append(" -> ");
         dot.append(condId);
         dot.append(" [label=\"ON\"];\n");
         dot.append(condExpr->toDotGraph(depth + 1, idGen));
      }
   } else if (std::holds_alternative<std::vector<std::shared_ptr<ColumnRefExpression>>>(condition)) {
      auto usingCols = std::get<std::vector<std::shared_ptr<ColumnRefExpression>>>(condition);
      for (size_t i = 0; i < usingCols.size(); ++i) {
         if (usingCols[i]) {
            std::string colId;
            colId.append("node");
            colId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(usingCols[i].get()))));

            dot.append(nodeId);
            dot.append(" -> ");
            dot.append(colId);
            dot.append(" [label=\"USING\"];\n");
            dot.append(usingCols[i]->toDotGraph(depth + 1, idGen));
         }
      }
   }

   return dot;
}
} // namespace lingodb::ast