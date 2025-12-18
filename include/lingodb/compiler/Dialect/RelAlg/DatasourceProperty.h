#ifndef LINGODB_DATASOURCEPROPERTY_H
#define LINGODB_DATASOURCEPROPERTY_H
#include "lingodb/runtime/storage/TableStorage.h"
#include "lingodb/utility/Serialization.h"
#include <llvm/ADT/Hashing.h>

namespace lingodb {
struct DatasourceProperty {
   std::string restrictions{""};
   std::vector<runtime::FilterDescription> filterDescription;

   inline bool operator==(const DatasourceProperty& other) const noexcept {
      return restrictions == other.restrictions && filterDescription == other.filterDescription;
   }

   void serialize(lingodb::utility::Serializer& serializer) {
      serializer.writeProperty(0, filterDescription);

   }

   static DatasourceProperty deserialize(lingodb::utility::Deserializer& deserializer) {
      std::vector<runtime::FilterDescription> filters = deserializer.readProperty<std::vector<runtime::FilterDescription>>(0);;
      return DatasourceProperty{.filterDescription = filters};
   }

   inline llvm::hash_code hash() const {
      return llvm::hash_combine(restrictions);
   }
};
} // namespace lingodb
namespace llvm {
inline hash_code hash_value(const lingodb::DatasourceProperty& datasource) {
   return datasource.hash();
}
} // namespace llvm
#endif //LINGODB_DATASOURCEPROPERTY_H
