#include "scoring/yakus/riichi.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isRiichi(const GameState& state, int player,
              const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.hands.at(player).riichi && !state.hands.at(player).open &&
         (state.turnNum > 4 || state.lastCall < 0);
}

}  // namespace mahjong::yaku
