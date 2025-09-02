#pragma once
#include <map>
#include <vector>

#include "analysis/analysis.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/sets.h"

namespace mahjong {

int countSingles(const std::vector<Piece>& hand);

int countPiece(const GameState& state, int player, Piece p);

std::vector<Piece> completeSet(Piece a, Piece b);

// Returns list of waits that complete the hand.
// Use with 13 piece hands.
std::vector<Piece> getWaits(const Hand& hand);

// Returns list of waits that complete the hand with piece removed.
// Use with 14 piece hands.
std::vector<Piece> getWaits(const Hand& hand, const Piece& piece);

// Returns a map of waits for the discard of a given piece that complete the
// hand.
// Use with 14 piece hands.
std::map<Piece, std::vector<Piece>> getPossibleWaits(const Hand& hand);

using SetCheckFunc = std::function<bool(SetType, Piece)>;
// Calls func on each set in branch, returns true if and only if func returns 
// true for all sets. Exits immediately on false.
bool CheckBranch(const Branch& branch, const SetCheckFunc& func);

}  // namespace mahjong
