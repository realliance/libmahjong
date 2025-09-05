#pragma once

#include <functional>

#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"

namespace mahjong {
using DecisionFunction =
    std::function<bool(const mahjong::GameState& state, const Player& player)>;

struct PossibleDecision {
  Event::Type type;
  DecisionFunction func;
};

bool CanKan(const GameState& state, const Player& player);
bool CanPon(const GameState& state, const Player& player);
bool CanChi(const GameState& state, const Player& player);
bool CanRon(const GameState& state, const Player& player);

bool CanTsumo(const GameState& state, const Player& player);
bool CanConcealedKan(const GameState& state, const Player& player);
bool CanConvertedKan(const GameState& state, const Player& player);
bool CanRiichi(const GameState& state, const Player& player);
}  // namespace mahjong
