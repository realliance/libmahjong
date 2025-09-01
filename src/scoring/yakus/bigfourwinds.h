#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"

namespace mahjong::yaku {
bool isBigFourWinds(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& branch);

}
