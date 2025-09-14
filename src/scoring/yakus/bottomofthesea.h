#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/hand.h"

namespace mahjong::yaku {
bool isBottomOfTheSea(const GameState& state, const Hand& hand,
                      const std::vector<const mahjong::Node*>& /*unused*/ = {});

}
