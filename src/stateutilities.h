#pragma once
#include <cstdint>

#include "event.h"
#include "gamestate.h"
#include "piecetype.h"
#include "winds.h"

namespace mahjong {

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
