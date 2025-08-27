#include "scoring/yakus/halfflush.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "scoring/yakus/fullflush.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isHalfFlush(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  const Hand& hand = state.hands[player];
  const int suit = hand.live.front().getSuit();
  bool honors = false;
  for (const auto& piece : hand.live) {
    if (piece.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != piece.getSuit()) {
      return false;
    }
  }
  for (int i = 0; i < hand.meld_count; ++i) {
    if (hand.melds[i].start.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != hand.melds[i].start.getSuit()) {
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
