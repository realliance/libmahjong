#include "scoring/yakus/pinfu.h"

#include <algorithm>
#include <vector>

#include "analysis/handnode.h"
#include "analysis/util.h"
#include "scoring/yakus.h"
#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isPinfu(const GameState& state, int player,
             const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  for (const auto* node : branch) {
    switch (node->type()) {
      case SetType::kPon:
      case SetType::kConcealedKan:
      case SetType::kKan:
      case SetType::kSingle:
        return false;

      case SetType::kChi:
      case SetType::kPair: {
        if (node->start() == kRedDragon || node->start() == kWhiteDragon ||
            node->start() == kGreenDragon) {
          return false;
        }
        if (node->start() == kSouthWind && state.roundNum > 3) {
          return false;
        }
        if (node->start() == kEastWind && state.roundNum < 4) {
          return false;
        }
        if (node->start() == Piece::fromWind(GetSeat(state.roundNum, player))) {
          return false;
        }
      }
    }
  }
  std::vector<Piece> hand = state.hands.at(player).live;
  hand.erase(std::find(hand.begin(), hand.end(), state.pendingPiece));
  return isInTenpai13Pieces(hand, /*allWaits=*/true).size() > 1;
}

REGISTER_YAKU({
    .id = "pinfu",
    .name = "Pinfu",
    .type = Yaku::kClosed,
    .value = 1,
    .is_yaku_func = yaku::isPinfu,
});
}  // namespace mahjong::yaku
