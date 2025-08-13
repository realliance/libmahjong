#include "scoring/yakus/allpons.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isAllPons(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& branch) {
  int pons = 0;
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kKan || meld.type == Meld::kPon ||
        meld.type == Meld::kConcealedKan) {
      pons++;
    }
  }
  return pons == 4;
}

}  // namespace mahjong::yakus
