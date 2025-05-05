#include "lingodb/compiler/frontend/sql-parser/result_modifier.h"
namespace lingodb::ast {
std::string ResultModifier::toAsciiAST(uint32_t depth) {
   return "ResultModifier: Not implemented";
}
std::string ResultModifier::toDotGraph(uint32_t depth) {
   return "ResultModifier: Not implemented";
}
std::string OrderByModifier::toAsciiAST(uint32_t depth) {
   return ResultModifier::toAsciiAST(depth);
}
std::string OrderByModifier::toDotGraph(uint32_t depth,  NodeIdGenerator& idGen) {
   std::string dot{};
   // Create node identifier
   std::string nodeId;
   nodeId.append("node");
   nodeId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))));

   // Create the order by node
   dot.append(nodeId);
   dot.append(" [label=\"ORDER BY\"];\n");

   // Add each order by element
   for (size_t i = 0; i < orderByElements.size(); ++i) {
      const auto& element = orderByElements[i];
      if (element->expression) {
         std::string elementId;
         elementId.append("node");
         elementId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(element->expression.get()))));

         // Create edge with ordering information
         dot.append(nodeId);
         dot.append(" -> ");
         dot.append(elementId);
         dot.append(" [label=\"");
         dot.append(std::to_string(i + 1));
         dot.append(": ");
         dot.append(element->type == OrderType::ASCENDING ? "ASC" : "DESC");
         //dot.append(element.nullsFirst ? " NULLS FIRST" : " NULLS LAST");
         dot.append("\"];\n");

         // Add the expression's graph
         dot.append(element->expression->toDotGraph(depth + 1, idGen));
      }
   }
   return dot;
}
} // namespace lingodb::ast