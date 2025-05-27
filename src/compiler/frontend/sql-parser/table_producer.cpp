#include "lingodb/compiler/frontend/sql-parser/table_producer.h"
namespace lingodb::ast {
TableProducer::TableProducer(NodeType type) : AstNode(type) {
}
TableProducer::TableProducer(NodeType type, std::string alias) : AstNode(type), alias(std::move(alias)) {
}

} // namespace lingodb::ast