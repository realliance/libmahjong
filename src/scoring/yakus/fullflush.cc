#include "scoring/yakus/fullflush.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isFullFlush(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).live.front().isHonor()) {
    return false;
  }
  const int suit = state.hands.at(player).live.front().getSuit();
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.getSuit() != suit) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.getSuit() != suit) {
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
