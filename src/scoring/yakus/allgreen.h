#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/hand.h"

namespace mahjong::yaku {
bool isAllGreen(const GameState& state, const Player& player,
                const std::vector<const mahjong::Node*>& /*unused*/ = {});

}
