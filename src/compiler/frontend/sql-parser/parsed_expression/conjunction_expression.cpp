#include "lingodb/compiler/frontend/sql-parser/parsed_expression/conjunction_expression.h"

#include "lingodb/compiler/frontend/sql-parser/query_node.h"
namespace lingodb::ast {

ConjunctionExpression::ConjunctionExpression(ExpressionType type): ParsedExpression(type, TYPE), children() {}
ConjunctionExpression::ConjunctionExpression(ExpressionType type, std::shared_ptr<lingodb::ast::ParsedExpression> left, std::shared_ptr<lingodb::ast::ParsedExpression> right) : ConjunctionExpression(type, std::vector{left,right}) {}
 ConjunctionExpression::ConjunctionExpression(ExpressionType type, std::vector<std::shared_ptr<ParsedExpression>> children) : ParsedExpression(type, TYPE), children(std::move(children)) {
}

std::string ConjunctionExpression::typeToAscii(ExpressionType type) const{
   switch (type) {
      case ExpressionType::CONJUNCTION_AND: return "AND";
      case ExpressionType::CONJUNCTION_OR: return "OR";
      default: return "Not found";
   }
}

std::string ConjunctionExpression::toAsciiAST(uint32_t depth )  {
   toAsciiASTPrefix
   ast.append("ConjunctionExpression: ");
   ast.append(typeToAscii(type));
   ast.append("\n");
   for (auto& child : children) {
      ast.append(child->toAsciiAST(depth+1));
   }
   return ast;
 };
std::string ConjunctionExpression::toDotGraph(uint32_t depth) {
    std::string dot{};
    
    // Create node identifier for the conjunction
    std::string nodeId;
    nodeId.append("node");
    nodeId.append(std::to_string(reinterpret_cast<uintptr_t>(this)));
    
    // Create the conjunction node
    dot.append(nodeId);
    dot.append(" [label=\"");
    dot.append(typeToAscii(type));
    dot.append("\"];\n");
    
    // Add all child expressions
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i]) {
            std::string childId;
            childId.append("node");
            childId.append(std::to_string(reinterpret_cast<uintptr_t>(children[i].get())));
            
            // Create edge from conjunction to this child
            dot.append(nodeId);
            dot.append(" -> ");
            dot.append(childId);
            dot.append(" [label=\"child ");
            dot.append(std::to_string(i + 1));
            dot.append("\"];\n");
            
            // Add the child's graph representation
            dot.append(children[i]->toDotGraph(depth + 1));
        }
    }
    
    return dot;
}
} // namespace lingodb::ast