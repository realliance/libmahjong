#pragma once

#include <functional>

#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"

namespace mahjong {
using DecisionFunction =
    std::function<bool(const mahjong::GameState& state, const Hand& hand)>;

struct PossibleDecision {
  Event::Type type;
  DecisionFunction func;
};

bool CanKan(const GameState& state, const Hand& hand);
bool CanPon(const GameState& state, const Hand& hand);
bool CanChi(const GameState& state, const Hand& hand);
bool CanRon(const GameState& state, const Hand& hand);

bool CanTsumo(const GameState& state, const Hand& hand);
bool CanConcealedKan(const GameState& state, const Hand& hand);
bool CanConvertedKan(const GameState& state, const Hand& hand);
bool CanRiichi(const GameState& state, const Hand& hand);
}  // namespace mahjong
