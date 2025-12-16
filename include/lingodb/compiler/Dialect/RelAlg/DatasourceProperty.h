#ifndef LINGODB_DATASOURCEPROPERTY_H
#define LINGODB_DATASOURCEPROPERTY_H
#include "lingodb/runtime/storage/TableStorage.h"
#include "lingodb/utility/Serialization.h"
namespace lingodb {
   struct DatasourceProperty {
      std::string restrictions;
      std::vector<runtime::FilterDescription> filterDescription;

      inline bool operator==(const DatasourceProperty& other) const {
         return false;
      }

      void serialize(lingodb::utility::Serializer& serializer) {
         serializer.writeProperty(0, restrictions);
         for (auto & filter : filterDescription) {
            filter.serialize(serializer);
         }
      }

      inline llvm::hash_code hash() const {
         return llvm::hash_combine(restrictions);
      }



   };
} // namespace lingodb
#endif //LINGODB_DATASOURCEPROPERTY_H
