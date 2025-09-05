#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"

namespace mahjong::yaku {
bool isRiichi(const GameState& state, const Player& player,
              const std::vector<const mahjong::Node*>& /*unused*/ = {});

}
