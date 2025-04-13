#pragma once
#include <cstdint>

#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {
struct GameState;

Wind GetSeat(int round, int player);

uint8_t RemovePieces(GameState& state, int player, Piece p, uint8_t count);
void DiscardPiece(GameState& state, int player, Piece p);

void AlertPlayers(const GameState& state, Event e);
Piece AskForDiscard(const GameState& state);
uint8_t CountPieces(const GameState& state, int player, Piece p);
bool ValidateDecision(const GameState& state, int player, Event decision,
                      bool inHand);
Event GetValidDecisionOrThrow(const GameState& state, int player, bool inHand);

}  // namespace mahjong
