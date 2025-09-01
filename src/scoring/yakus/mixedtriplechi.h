#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"

namespace mahjong::yaku {
bool isMixedTripleChi(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch);

}
