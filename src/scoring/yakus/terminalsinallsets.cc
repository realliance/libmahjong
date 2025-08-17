#include "scoring/yakus/terminalsinallsets.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isTerminalsInAllSets(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  for (const auto& node : branch) {
    switch (node->type()) {
      case Node::kError:
      case Node::kSingle:
        return false;
      case Node::kRoot:
        break;
      case Node::kChiSet:
        if (!node->start().isTerminal() && !(node->start() + 2).isTerminal()) {
          return false;
        }
        break;
      case Node::kPair:
      case Node::kPonSet:
        if (!node->start().isTerminal()) {
          return false;
        }
        break;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    switch (meld.type) {
      case Meld::kChi:
        if (!meld.start.isTerminal() && !(meld.start + 2).isTerminal()) {
          return false;
        }
        break;
      case Meld::kKan:
      case Meld::kConcealedKan:
      case Meld::kPon:
        if (!meld.start.isTerminal()) {
          return false;
        }
        break;
    }
  }
  return true;
}

}  // namespace mahjong::yaku
