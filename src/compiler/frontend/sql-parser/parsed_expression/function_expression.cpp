#include "lingodb/compiler/frontend/sql-parser/parsed_expression/function_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {
FunctionExpression::FunctionExpression(std::string catalog, std::string schema, std::string functionName, bool isOperator, bool distinct, bool exportState) : ParsedExpression(ExpressionType::FUNCTION, TYPE), catalog(catalog), schema(schema), functionName(functionName), isOperator(isOperator), distinct(distinct), exportState(exportState) {
   auto found = std::find(aggregationFunctions.begin(), aggregationFunctions.end(), functionName);
   if (found != aggregationFunctions.end()) {
      //! TODO Check if this make sense here
      this->type = ExpressionType::AGGREGATE;
   }
}
std::string FunctionExpression::toAsciiAST(uint32_t depth) {
   toAsciiASTPrefix
      ast.append("FunctionExpression: ");
   ast.append(functionName);
   ast.append("\n");
   for (auto& argument : arguments) {
      ast.append(argument->toAsciiAST(depth + 1));
   }

   return ast;
}
std::string FunctionExpression::toDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot{};
   // Create node identifier for the function
   std::string nodeId;
   nodeId.append("node");
   nodeId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))));

   // Create the function node with its label
   dot.append(nodeId);
   dot.append(" [label=\"Function\nname: ");
   dot.append(functionName);
   if (distinct) {
      dot.append("\\nDISTINCT");
   }
   if (type == ExpressionType::AGGREGATE) {
      dot.append("\nagg\n");
   }
   dot.append("\"];\n");

   // Add all function arguments
   for (size_t i = 0; i < arguments.size(); ++i) {
      if (arguments[i]) {
         std::string argId;
         argId.append("node");
         argId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(arguments[i].get()))));

         // Create edge from function to this argument
         dot.append(nodeId);
         dot.append(" -> ");
         dot.append(argId);
         dot.append(" [label=\"arg ");
         dot.append(std::to_string(i + 1));
         dot.append("\"];\n");

         // Add the argument's graph representation
         dot.append(arguments[i]->toDotGraph(depth + 1, idGen));
      }
   }

   // Add filter if present
   if (filter) {
      std::string filterId;
      filterId.append("node");
      filterId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(filter.get()))));

      dot.append(nodeId);
      dot.append(" -> ");
      dot.append(filterId);
      dot.append(" [label=\"filter\"];\n");
      dot.append(filter->toDotGraph(depth + 1, idGen));
   }

   // Add order by if present
   if (orderBy) {
      std::string orderId;
      orderId.append("node");
      orderId.append(std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(orderBy.get()))));

      dot.append(nodeId);
      dot.append(" -> ");
      dot.append(orderId);
      dot.append(" [label=\"order by\"];\n");
      dot.append(orderBy->toDotGraph(depth + 1, idGen));
   }

   return dot;
}

} // namespace lingodb::ast