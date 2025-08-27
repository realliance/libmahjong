#include "scoring/yakus/fullflush.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isFullFlush(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  const Hand& hand = state.hands[player];
  if (hand.live.front().isHonor()) {
    return false;
  }
  const int suit = hand.live.front().getSuit();
  for (const auto& piece : hand.live) {
    if (piece.getSuit() != suit) {
      return false;
    }
  }
  for (int i = 0; i < hand.meld_count; ++i) {
    if (hand.melds[i].start.getSuit() != suit) {
      return false;
    }
  }
  return true;
}

REGISTER_YAKU({
    .id = "fullflush",
    .name = "Full Flush",
    .type = Yaku::kBonusWhenClosed,
    .value = 5,
    .is_yaku_func = yaku::isFullFlush,
});
}  // namespace mahjong::yaku
