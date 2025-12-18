#ifndef LINGODB_EXTERNALDATASOURCEPROPERTY_H
#define LINGODB_EXTERNALDATASOURCEPROPERTY_H
#include "lingodb/utility/Serialization.h"
#include "storage/TableStorage.h"
namespace lingodb {
struct ExternalDatasourceProperty {
   struct Mapping {
      std::string memberName;
      std::string identifier;

   };
   std::string tableName;
   std::vector<Mapping> mapping;
   ;
   std::vector<runtime::FilterDescription> filterDescriptions;

   void serialize(lingodb::utility::Serializer& serializer) {
      serializer.writeProperty(0, tableName);
      serializer.writeProperty(1, mapping.size());
      size_t idx = 2;
      for (auto& map : mapping) {
         serializer.writeProperty(idx, map.memberName);
         serializer.writeProperty(idx + 1, map.identifier);
         idx += 2;
      }
      serializer.writeProperty(idx, filterDescriptions.size());
      idx++;
      for (auto& filterDescription : filterDescriptions) {
         filterDescription.serialize(serializer, idx);
      }

   }

   static ExternalDatasourceProperty deserialize(lingodb::utility::Deserializer& deserializer) {
      ExternalDatasourceProperty prop{};
      prop.tableName = deserializer.readProperty<std::string>(0);
      size_t mappingCount = deserializer.readProperty<size_t>(1);
      size_t propIdx = 2;
      for (size_t i = 0; i < mappingCount; ++i) {
         prop.mapping.emplace_back(deserializer.readProperty<std::string>(propIdx), deserializer.readProperty<std::string>(propIdx + 1));
         propIdx += 2;
      }
      size_t filterCount = deserializer.readProperty<size_t>(propIdx);
      propIdx++;

      for (size_t i = 0; i < filterCount; ++i) {
         prop.filterDescriptions.emplace_back(runtime::FilterDescription::deserialize(deserializer, propIdx));
      }


      return prop;
   }
};
} // namespace lingodb

#endif //LINGODB_EXTERNALDATASOURCEPROPERTY_H
