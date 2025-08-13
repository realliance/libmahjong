#include "scoring/yakus/fourkans.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isFourKans(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*branch*/) {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kKan) {
      kans++;
    }
  }
  return kans == 4;
}

}  // namespace mahjong::yaku
