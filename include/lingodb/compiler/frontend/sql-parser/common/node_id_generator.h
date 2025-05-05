#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
namespace lingodb::ast {
class NodeIdGenerator {
   public:
   void reset();

   uint64_t getId(uintptr_t id);

   private:
   uint64_t currentId;
   std::unordered_map<uint64_t, uint64_t> idMap;
};

} // namespace lingodb::ast