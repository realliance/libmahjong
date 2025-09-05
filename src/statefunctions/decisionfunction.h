#pragma once

#include <functional>

#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"

namespace mahjong {
using DecisionFunction =
    std::function<bool(const mahjong::GameState& state, const Hand& player)>;

struct PossibleDecision {
  Event::Type type;
  DecisionFunction func;
};

bool CanKan(const GameState& state, const Hand& player);
bool CanPon(const GameState& state, const Hand& player);
bool CanChi(const GameState& state, const Hand& player);
bool CanRon(const GameState& state, const Hand& player);

bool CanTsumo(const GameState& state, const Hand& player);
bool CanConcealedKan(const GameState& state, const Hand& player);
bool CanConvertedKan(const GameState& state, const Hand& player);
bool CanRiichi(const GameState& state, const Hand& player);
}  // namespace mahjong
