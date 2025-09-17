#include "scoring/yakus/ninegates.h"

#include <map>
#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "scoring/yakus/fullflush.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isNineGates(const GameState& state, const Hand& hand,
                 const std::vector<const mahjong::Node*>& branch) {
  if (hand.open) {
    return false;
  }
  if (!isFullFlush(state, hand, branch)) {
    return false;
  }
  if (hand.open) {
    return false;
  }
  std::map<int, int> pieces;
  for (int i = 1; i < 10; i++) {
    pieces.at(i) = 0;
  }
  bool duplicate = false;
  for (const auto& piece : hand.live_range()) {
    if (pieces.contains(piece.getPieceNum())) {
      if ((pieces[piece.getPieceNum()] != 0) && !piece.isTerminal()) {
        if (duplicate) {
          return false;
        }
        duplicate = true;
      } else {
        pieces[piece.getPieceNum()]++;
      }
    }
  }
  if (!duplicate) {
    return false;
  }
  for (const auto& [piece, count] : pieces) {
    if ((piece == 1 || piece == 9)) {
      if (count != 3) {
        return false;
      }
    } else if (count != 1) {
      return false;
    }
  }
  return true;
}

REGISTER_YAKU({
    .id = "ninegates",
    .name = "Nine Gates",
    .type = Yaku::kYakuman,
    .value = 1,
    .is_yaku_func = yaku::isNineGates,
});
}  // namespace mahjong::yaku
