#ifndef LINGODB_RUNTIME_CATALOG_H
#define LINGODB_RUNTIME_CATALOG_H
#include "Relation.h"
namespace lingodb::runtime {
class Catalog {
   public:
   virtual std::shared_ptr<Relation> findRelation(std::string name) = 0;
   virtual void addTable(std::string tableName, std::shared_ptr<TableMetaData> mD)=0;
   virtual void setPersist(bool value)=0;

   virtual ~Catalog() {}
   static std::shared_ptr<Catalog> createEmpty();
};
class LocalCatalog : public Catalog {
   std::shared_ptr<Catalog> nested;

   LocalCatalog(std::shared_ptr<Catalog> nested) : nested(nested) {}

   public:
   std::unordered_map<std::string, std::shared_ptr<Relation>> relations;

   static std::shared_ptr<LocalCatalog> create(std::shared_ptr<Catalog> nested);
   std::shared_ptr<Relation> findRelation(std::string name) override;
   void addTable(std::string tableName, std::shared_ptr<TableMetaData> mD) override;
   void setPersist(bool value) override;

};
class DBCatalog : public Catalog {
   bool persist=false;
   std::shared_ptr<Catalog> nested;
   std::string dbDirectory;
   DBCatalog(std::shared_ptr<Catalog> nested) : nested(nested) {}

   public:
   std::unordered_map<std::string, std::shared_ptr<Relation>> relations;
   std::shared_ptr<Relation> findRelation(std::string name) override;
   static std::shared_ptr<DBCatalog> create(std::shared_ptr<Catalog> nested, std::string dbDir,bool eagerLoading);
   void addTable(std::string tableName, std::shared_ptr<TableMetaData> mD) override;
   void setPersist(bool value) override;

};

} // end namespace lingodb::runtime

#endif //LINGODB_RUNTIME_CATALOG_H
