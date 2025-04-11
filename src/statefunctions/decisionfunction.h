#pragma once

#include <functional>

#include "types/event.h"
#include "types/gamestate.h"

namespace mahjong {
using DecisionFunction =
    std::function<bool(const mahjong::GameState& state, int player)>;

struct GameState;

struct PossibleDecision {
  Event::Type type;
  DecisionFunction func;
};

bool CanKan(const GameState& state, int player);
bool CanPon(const GameState& state, int player);
bool CanChi(const GameState& state, int player);
bool CanRon(const GameState& state, int player);

bool CanTsumo(const GameState& state, int player);
bool CanConcealedKan(const GameState& state, int player);
bool CanConvertedKan(const GameState& state, int player);
bool CanRiichi(const GameState& state, int player);
}  // namespace mahjong
