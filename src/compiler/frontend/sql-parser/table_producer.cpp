#include "lingodb/compiler/frontend/sql-parser/table_producer.h"
namespace lingodb::ast {
TableProducer::TableProducer(NodeType type) : AstNode(type) {
}
std::string TableProducer::inputToDotGraph(uint32_t depth, NodeIdGenerator& idGen) {
   std::string dot{};
   if (input) {
      dot += "node" + std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(this))) +
         " -> node" + std::to_string(idGen.getId(reinterpret_cast<uintptr_t>(input.get()))) + "[label=\"has input\"];\n";
      dot += input->toDotGraph(depth + 1, idGen);
   }
   return dot;
}
} // namespace lingodb::ast