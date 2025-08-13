#pragma once
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isSeatWind(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch);

bool isPrevalentWind(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch);

bool isGreenDragon(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch);

bool isRedDragon(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch);

bool isWhiteDragon(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch);
}  // namespace mahjong::yakus
