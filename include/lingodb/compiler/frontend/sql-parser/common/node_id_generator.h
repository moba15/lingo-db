#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
namespace lingodb::ast {
class NodeIdGenerator {
   public:
   void reset();

   uint64_t getId(uintptr_t id);

   void setId(uint64_t id, uint64_t setID);

   private:
   uint64_t currentId;
   std::unordered_map<uint64_t, uint64_t> idMap;
};

} // namespace lingodb::ast