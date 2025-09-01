#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"

namespace mahjong::yaku {
bool isMaxBranches(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*unused*/ = {});

}
