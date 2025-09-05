#include "scoring/yakus/fullflush.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isFullFlush(const GameState&  /*state*/, const Player& player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
    if (player.live[0].isHonor()) {
    return false;
  }
  const int suit = player.live[0].getSuit();
  for (const auto& piece : player.live_range()) {
    if (piece.getSuit() != suit) {
      return false;
    }
  }
  for (const auto& meld : player.melds_range()) {
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
