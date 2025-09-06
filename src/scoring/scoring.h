#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/score.h"

namespace mahjong {
Score scoreHand(const GameState& state, const Hand& player);

int getBasicPoints(Score s);

int getFu(const GameState& state, const Hand& player,
          const std::vector<const mahjong::Node*>& branch);

bool isOpenPinfu(const GameState& state, const Hand& player,
                 const std::vector<const mahjong::Node*>& branch);

bool isComplete(const GameState& state, const Hand& player);
}  // namespace mahjong
