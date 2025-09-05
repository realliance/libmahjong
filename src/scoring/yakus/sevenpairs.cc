#include "scoring/yakus/sevenpairs.h"

#include <set>
#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isSevenPairs(const GameState& /* state */, const Player& player,
                  const std::vector<const mahjong::Node*>& branch) {
  if (player.open) {
    return false;
  }
  std::set<Piece> pairs;
  for (const auto* node : branch) {
    switch (node->type()) {
      case SetType::kChi:
      case SetType::kPon:
      case SetType::kConcealedKan:
      case SetType::kKan:
      case SetType::kSingle:
        return false;
      case SetType::kPair:
        pairs.insert(node->start());
    }
  }
  return pairs.size() == 7;
}

REGISTER_YAKU({
    .id = "sevenpairs",
    .name = "Seven Pairs",
    .type = Yaku::kClosed,
    .value = 2,
    .is_yaku_func = yaku::isSevenPairs,
});
}  // namespace mahjong::yaku
