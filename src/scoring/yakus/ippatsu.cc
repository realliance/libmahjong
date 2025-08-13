#include "scoring/yakus/ippatsu.h"

#include <vector>

#include "scoring/yakus/doubleriichi.h"
#include "scoring/yakus/riichi.h"
#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isIppatsu(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& /*branch*/) {
  return (isRiichi(state, player) || isDoubleRiichi(state, player)) &&
         (state.turnNum - state.hands.at(player).riichiRound <= 4 &&
          state.lastCall < state.hands.at(player).riichiRound);
}

}  // namespace mahjong::yakus
