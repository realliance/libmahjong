#include "statefunctions/router.h"

#include <utility>

#include "types/statefunction.h"

namespace mahjong {
StateFunctionFunc Router::Route(const StateFunctionType& type) {
  return routes_[type];
}

bool Router::RegisterRoute(StateFunctionFunc func,
                           const StateFunctionType& type) {
  if (routes_.contains(type)) {
    return false;
  }
  routes_[type] = std::move(func);
  return true;
}
}  // namespace mahjong
