#pragma once

#include <functional>
#include "analysis/handnode.h"
#include "types/gamestate.h"

namespace mahjong {

using yakuFunc = std::function<int(const mahjong::GameState&, int,
                                   const std::vector<const mahjong::Node*>&)>;

using Han = int;

struct Yaku {  // add yakus for exclusions, name, and enum possibly
  enum Type {
    kOpen,
    kClosed,
    kBonusWhenClosed,
    kYakuman,
  };
  const std::string id;
  const std::string name;
  const Type type;
  const Han value;
  const yakuFunc is_yaku_func;
};
}  // namespace mahjong
