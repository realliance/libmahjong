#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/hand.h"

namespace mahjong::yaku {
bool isAllHonors(const GameState& state, const Hand& player,
                 const std::vector<const mahjong::Node*>& /*unused*/ = {});

}
