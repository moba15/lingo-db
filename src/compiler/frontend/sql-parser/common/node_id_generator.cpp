#include "lingodb/compiler/frontend/sql-parser/common/node_id_generator.h"

#include <memory>
namespace lingodb::ast {
void NodeIdGenerator::reset() {
   currentId = 0;
   idMap.clear();
}

uint64_t NodeIdGenerator::getId(uintptr_t id) {
   if (!idMap.contains(id)) {
      idMap[id] = currentId;
      currentId++;
   }
   return idMap[id];
}

void NodeIdGenerator::setId(uint64_t id, uint64_t setID) {
   idMap[id] = setID;
}
} // namespace lingodb::ast