#pragma once
#include <cstdint>

#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {
struct GameState;

Wind GetSeat(int round, int player);

uint8_t RemovePieces(Hand& player, Piece p, uint8_t count);
void DiscardPiece(Hand& player, Piece p);

void AlertPlayers(const GameState& state, Event e);
Piece AskForDiscard(const GameState& state);
uint8_t CountPieces(const Hand& player, Piece p);
bool ValidateDecision(const GameState& state, const Hand& player,
                      Event decision, bool inHand);
Event GetValidDecisionOrThrow(const GameState& state, const Hand& player,
                              bool inHand);

}  // namespace mahjong
