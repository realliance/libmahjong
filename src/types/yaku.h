#pragma once

#include <functional>
#include "types/gamestate.h"
#include "types/handnode.h"

#include "scoring/yakus.h"

namespace mahjong {

using yakuFunc = std::function<int(const mahjong::GameState&, int,
                                   const std::vector<const mahjong::Node*>&)>;

struct Yaku { // add yakus for exclusions, name, and enum possibly
  const YakuId id;
  const std::string name;
  const yakuFunc is_yaku_func;
  const std::vector<YakuId> doesnt_score_with;
  const int han;
  const bool bonus_when_closed;
};
}  // namespace mahjong
