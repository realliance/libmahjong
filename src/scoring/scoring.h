#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/score.h"

namespace mahjong {
Score scorePlayer(const GameState& state, const Player& player);

int getBasicPoints(Score s);

int getFu(const GameState& state, const Player& player,
          const std::vector<const mahjong::Node*>& branch);

bool isOpenPinfu(const GameState& state, const Player& player,
                 const std::vector<const mahjong::Node*>& branch);

bool isComplete(const GameState& state, const Player& player);
}  // namespace mahjong
