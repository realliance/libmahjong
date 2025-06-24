#pragma once
#include <map>
#include <memory>

#include "types/statefunction.h"

namespace mahjong {
class Router {
 public:
  static Router& Instance() {
    static Router router;
    return router;
  }

  StateFunctionFunc Route(const StateFunctionType& type);
  bool RegisterRoute(StateFunctionFunc func, const StateFunctionType& type);

 private:
  std::map<StateFunctionType, StateFunctionFunc> routes_;
};

#define REGISTER_ROUTE(func, type)                                   \
  namespace {                                                        \
  bool __registered = Router::Instance().RegisterRoute(&func, type); \
  }

}  // namespace mahjong
