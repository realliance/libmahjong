#include "scoring/yakus/maxbranches.h"

#include <array>
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/sets.h"

namespace mahjong::yaku {
bool isMaxBranches(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*branch*/) {
  const Hand& hand = state.hands[player];
  const int start = hand.live[0].getPieceNum();
  const int suit = hand.live[0].getSuit();
  std::array<int, 6> sets = {};
  const std::array<int, 6> final = {3, 3, 2, 2, 2, 2};

  for (const auto& piece : hand.live_range()) {
    if (piece.getSuit() != suit) {
      return false;
    }
    if (piece.getPieceNum() < start || piece.getPieceNum() > start + 5) {
      return false;
    }
    sets.at(piece.getPieceNum() - start)++;
  }
  for (int i = 0; i < hand.meld_count; ++i) {
    if (hand.melds[i].start.getSuit() != suit) {
      return false;
    }
    if (hand.melds[i].start.getPieceNum() < start ||
        hand.melds[i].start.getPieceNum() > start + 3) {
      return false;
    }
    if (hand.melds[i].type >= SetType::kKan) {
      if (hand.melds[i].start.getPieceNum() != start &&
          hand.melds[i].start.getPieceNum() != start + 1) {
        return false;
      }
      sets.at(hand.melds[i].start.getPieceNum() - start) += 3;
    }
    if (hand.melds[i].type == SetType::kPon) {
      sets.at(hand.melds[i].start.getPieceNum() - start) += 3;
    }
    if (hand.melds[i].type == SetType::kChi) {
      sets.at(hand.melds[i].start.getPieceNum() - start)++;
      sets.at(hand.melds[i].start.getPieceNum() - start + 1)++;
      sets.at(hand.melds[i].start.getPieceNum() - start + 2)++;
    }
  }
  for (int i = 0; i < 6; i++) {
    if (sets.at(i) != final.at(i)) {
      return false;
    }
  }
  return true;
}

// REGISTER_YAKU({
//     .id = "maxbranches",
//     .name = "Max Branches",
//     .type = Yaku::kYakuman,
//     .value = 13,
//     .is_yaku_func = yaku::isMaxBranches,
// });
}  // namespace mahjong::yaku
