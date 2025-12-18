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
      serializer.writeProperty(0, filterDescription.size());
      size_t offset = 1;
      for (auto& filter : filterDescription) {
         filter.serialize(serializer, offset);
      }
   }

   static DatasourceProperty deserialize(lingodb::utility::Deserializer& deserializer) {
      size_t filterCount = deserializer.readProperty<size_t>(0);
      std::vector<runtime::FilterDescription> filters{filterCount};
      size_t offset = 1;
      for (size_t i = 0; i < filterCount; ++i) {
         filters[i] = (runtime::FilterDescription::deserialize(deserializer, offset));
      }
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
