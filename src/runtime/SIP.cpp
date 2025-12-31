#include "lingodb/runtime/SIP.h"

#include "lingodb/compiler/Dialect/SubOperator/MemberManager.h"

#include <functional>
#include <string>

namespace lingodb::runtime {
std::unordered_map<std::string, lingodb::runtime::HashIndexedView*> SIP::filters{};
VarLen32 SIP::createSIP(DataSource* description, lingodb::runtime::HashIndexedView* hash, VarLen32  sipName) {
   // Create a hash based on the DataSource pointer so we get a stable identifier
   // for this DataSource instance. Use uintptr_t to avoid narrowing.
   std::uintptr_t key = reinterpret_cast<std::uintptr_t>(description);
   std::size_t h = std::hash<std::uintptr_t>()(key);
   filters.emplace(sipName.str(), hash);
   std::cerr << sipName.str() << std::endl;

   return VarLen32::fromString(sipName.str());
}
} // namespace lingodb::runtime