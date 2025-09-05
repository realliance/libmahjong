#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"

namespace mahjong::yaku {
bool isAllSimples(const GameState& state, const Player& player,
                  const std::vector<const mahjong::Node*>& /*unused*/ = {});

}
