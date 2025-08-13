#include "scoring/yakus/outsidehand.h"

#include <vector>

#include "scoring/yakus/allterminalsandhonors.h"
#include "scoring/yakus/terminalsinallsets.h"
#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isOutsideHand(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch) {
  bool chi = false;
  for (const auto& node : branch) {
    if (node->type() == Node::kChiSet) {
      if (node->start().isTerminal() || (node->start() + 2).isTerminal()) {
        chi = true;
      } else {
        return false;
      }
    } else {
      if (!node->start().isTerminal() && !node->start().isHonor()) {
        return false;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kChi) {
      if (meld.start.isTerminal() || (meld.start + 2).isTerminal()) {
        chi = true;
      } else {
        return false;
      }
    } else {
      if (!meld.start.isTerminal() && !meld.start.isHonor()) {
        return false;
      }
    }
  }
  // Terminals in all Sets and All Terminals and Honors are more valuable and score instead of Outside Hand.
  return chi && !isTerminalsInAllSets(state, player, branch) &&
         !isAllTerminalsAndHonors(state, player);
}

}  // namespace mahjong::yakus
