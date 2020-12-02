#pragma once
#include <cstdint>

#include "piecetype.h"
#include "winds.h"

namespace Mahjong {
struct Event;
struct GameState;

auto GetSeat(int round, int player) -> Wind;

auto RemovePieces(GameState& state, int player, Piece p, uint8_t count) -> uint8_t;
auto DiscardPiece(GameState& state, int player, Piece p) -> void;

auto AlertPlayers(const GameState& state, Event e) -> void;
auto AskForDiscard(const GameState& state) -> Piece;
auto CountPieces(const GameState& state, int player, Piece p) -> uint8_t;
auto ValidateDecision(const GameState& state, int player, Event decision, bool inHand) -> bool;
auto GetValidDecisionOrThrow(const GameState& state, int player, bool inHand) -> Event;

}  // namespace Mahjong
