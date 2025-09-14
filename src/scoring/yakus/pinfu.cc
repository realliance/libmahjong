#include "scoring/yakus/pinfu.h"

#include <vector>

#include "analysis/handnode.h"
#include "analysis/util.h"
#include "scoring/yakus.h"
#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/pieces.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isPinfu(const GameState& state, const Hand& hand,
             const std::vector<const mahjong::Node*>& branch) {
  if (hand.open) {
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
        if (node->start() ==
            Piece::fromWind(GetSeat(state.roundNum, hand.id))) {
          return false;
        }
      }
    }
  }
  return getWaits(hand, state.pendingPiece).size() >= 2;
}

REGISTER_YAKU({
    .id = "pinfu",
    .name = "Pinfu",
    .type = Yaku::kClosed,
    .value = 1,
    .is_yaku_func = yaku::isPinfu,
});
}  // namespace mahjong::yaku
