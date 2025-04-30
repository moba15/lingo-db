#pragma once
#include <string>
namespace lingodb::ast {
class BindingAlias {
   public:
   //TODO Add more constructures
   BindingAlias(std::string alias) : alias(alias) {}

   std::string alias;
};
} // namespace lingodb::ast