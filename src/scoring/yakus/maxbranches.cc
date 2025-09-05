#include "scoring/yakus/maxbranches.h"

#include <array>
#include <vector>

#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/sets.h"

namespace mahjong::yaku {
bool isMaxBranches(const GameState&  /*state*/, const Player& player,
                   const std::vector<const mahjong::Node*>& /*branch*/) {
    const int start = player.live[0].getPieceNum();
  const int suit = player.live[0].getSuit();
  std::array<int, 6> sets = {};
  const std::array<int, 6> final = {3, 3, 2, 2, 2, 2};

  for (const auto& piece : player.live_range()) {
    if (piece.getSuit() != suit) {
      return false;
    }
    if (piece.getPieceNum() < start || piece.getPieceNum() > start + 5) {
      return false;
    }
    sets.at(piece.getPieceNum() - start)++;
  }
  for (const auto& meld : player.melds_range()) {
    if (meld.start.getSuit() != suit) {
      return false;
    }
    if (meld.start.getPieceNum() < start ||
        meld.start.getPieceNum() > start + 3) {
      return false;
    }
    if (meld.type >= SetType::kKan) {
      if (meld.start.getPieceNum() != start &&
          meld.start.getPieceNum() != start + 1) {
        return false;
      }
      sets.at(meld.start.getPieceNum() - start) += 3;
    }
    if (meld.type == SetType::kPon) {
      sets.at(meld.start.getPieceNum() - start) += 3;
    }
    if (meld.type == SetType::kChi) {
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

// REGISTER_YAKU({
//     .id = "maxbranches",
//     .name = "Max Branches",
//     .type = Yaku::kYakuman,
//     .value = 13,
//     .is_yaku_func = yaku::isMaxBranches,
// });
}  // namespace mahjong::yaku
