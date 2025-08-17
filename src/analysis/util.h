#pragma once
#include <vector>

#include "types/gamestate.h"
#include "types/piecetype.h"

namespace mahjong {

std::vector<Piece> getRiichiDiscard(std::vector<Piece> hand);

std::vector<Piece> isInTenpai13Pieces(std::vector<Piece> hand, bool allWaits);

std::vector<Piece> isInTenpai(std::vector<Piece> hand, bool allWaits);

int countSingles(const std::vector<Piece>& hand);

int countPiece(const GameState& state, int player, Piece p);

}  // namespace mahjong
