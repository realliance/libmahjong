#pragma once
#include <cstdint>

#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {
struct GameState;

Wind GetSeat(int round, int player);

uint8_t RemovePieces(Player& player, Piece p, uint8_t count);
void DiscardPiece(Player& player, Piece p);

void AlertPlayers(const GameState& state, Event e);
Piece AskForDiscard(const GameState& state);
uint8_t CountPieces(const Player& player, Piece p);
bool ValidateDecision(const GameState& state, const Player& player,
                      Event decision, bool inPlayer);
Event GetValidDecisionOrThrow(const GameState& state, const Player& player,
                              bool inPlayer);

}  // namespace mahjong
