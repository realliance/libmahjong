#include "scoring/yakus/halfflush.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "scoring/yakus/fullflush.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isHalfFlush(const GameState& state, const Hand& hand,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  const int suit = hand.live[0].getSuit();
  bool honors = false;
  for (const auto& piece : hand.live_range()) {
    if (piece.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != piece.getSuit()) {
      return false;
    }
  }
  for (const auto& meld : hand.melds_range()) {
    if (meld.start.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != meld.start.getSuit()) {
      return false;
    }
  }
  // Full Flush scores instead of Half Flush.
  return honors && !isFullFlush(state, hand);
}

REGISTER_YAKU({
    .id = "halfflush",
    .name = "Half Flush",
    .type = Yaku::kBonusWhenClosed,
    .value = 1,
    .is_yaku_func = yaku::isHalfFlush,
});
}  // namespace mahjong::yaku
