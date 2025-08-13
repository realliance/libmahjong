#pragma once
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isFullyConcealedHand(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

}
