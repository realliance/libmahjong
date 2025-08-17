#pragma once
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isTriplePon(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch);

}
