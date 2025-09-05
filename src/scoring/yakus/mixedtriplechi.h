#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/hand.h"

namespace mahjong::yaku {
bool isMixedTripleChi(const GameState& state, const Hand& player,
                      const std::vector<const mahjong::Node*>& branch);

}
