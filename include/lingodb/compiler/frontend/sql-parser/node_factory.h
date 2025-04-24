#pragma once
#include <memory>
#include <vector>
#include "location.hh"
namespace lingodb::ast {
class NodeFactory {
public:
  template<class T, class... Args>
  std::shared_ptr<T> node(lingodb::location loc, Args... args) {
    auto node = std::make_shared<T>(std::forward<Args>(args)...);
    //TODO node->set_location(loc);
    return std::move(node);
  }
   template<class T, class... Args>
   std::vector<std::shared_ptr<T>> listShared() {
     std::vector<std::shared_ptr<T>> result{};
     return result;
  }

   template<class T, class... Args>
   std::vector<T> list() {
     std::vector<T> result{};
     return result;
  }
};
}