#include "scoring/yakus/halfflush.h"

#include <vector>

#include "scoring/yakus.h"
#include "scoring/yakus/fullflush.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isHalfFlush(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  const int suit = state.hands.at(player).live.front().getSuit();
  bool honors = false;
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != piece.getSuit()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != meld.start.getSuit()) {
      return false;
    }
  }
  // Full Flush scores instead of Half Flush.
  return honors && !isFullFlush(state, player);
}

REGISTER_YAKU({
    .id = "halfflush",
    .name = "Half Flush",
    .type = Yaku::kBonusWhenClosed,
    .value = 1,
    .is_yaku_func = yaku::isHalfFlush,
});
}  // namespace mahjong::yaku
