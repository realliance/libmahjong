#include "scoring/yakus/fourconcealedpon.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isFourConcealedPon(const GameState& state, int player,
                        const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  int concealed_pons = 0;
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      concealed_pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kConcealedKan) {
      concealed_pons++;
    }
  }
  return concealed_pons == 4;
}

}  // namespace mahjong::yakus
