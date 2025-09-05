#pragma once
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"

namespace mahjong::yaku {
bool isSeatWind(const GameState& state, const Player& player,
                const std::vector<const mahjong::Node*>& branch);

bool isPrevalentWind(const GameState& state, const Player& player,
                     const std::vector<const mahjong::Node*>& branch);

bool isGreenDragon(const GameState& state, const Player& player,
                   const std::vector<const mahjong::Node*>& branch);

bool isRedDragon(const GameState& state, const Player& player,
                 const std::vector<const mahjong::Node*>& branch);

bool isWhiteDragon(const GameState& state, const Player& player,
                   const std::vector<const mahjong::Node*>& branch);
}  // namespace mahjong::yaku
