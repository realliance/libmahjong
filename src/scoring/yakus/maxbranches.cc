#include "scoring/yakus/maxbranches.h"

#include <array>
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isMaxBranches(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*branch*/) {
  const int start = state.hands.at(player).live[0].getPieceNum();
  const int suit = state.hands.at(player).live[0].getSuit();
  std::array<int, 6> sets = {};
  const std::array<int, 6> final = {3, 3, 2, 2, 2, 2};
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.getSuit() != suit) {
      return false;
    }
    if (piece.getPieceNum() < start || piece.getPieceNum() > start + 5) {
      return false;
    }
    sets.at(piece.getPieceNum() - start)++;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.getSuit() != suit) {
      return false;
    }
    if (meld.start.getPieceNum() < start ||
        meld.start.getPieceNum() > start + 3) {
      return false;
    }
    if (meld.type >= Meld::kKan) {
      if (meld.start.getPieceNum() != start &&
          meld.start.getPieceNum() != start + 1) {
        return false;
      }
      sets.at(meld.start.getPieceNum() - start) += 3;
    }
    if (meld.type == Meld::kPon) {
      sets.at(meld.start.getPieceNum() - start) += 3;
    }
    if (meld.type == Meld::kChi) {
      sets.at(meld.start.getPieceNum() - start)++;
      sets.at(meld.start.getPieceNum() - start + 1)++;
      sets.at(meld.start.getPieceNum() - start + 2)++;
    }
  }
  for (int i = 0; i < 6; i++) {
    if (sets.at(i) != final.at(i)) {
      return false;
    }
  }
  return true;
}

}  // namespace mahjong::yakus
