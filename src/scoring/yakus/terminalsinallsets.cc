#include "scoring/yakus/terminalsinallsets.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isTerminalsInAllSets(const GameState&  /*state*/, const Player& player,
                          const std::vector<const mahjong::Node*>& branch) {
  for (const auto* node : branch) {
    switch (node->type()) {
      case SetType::kSingle:
        return false;
      case SetType::kChi:
        if (!node->start().isTerminal() && !(node->start() + 2).isTerminal()) {
          return false;
        }
        break;
      case SetType::kPair:
      case SetType::kPon:
      case SetType::kKan:
      case SetType::kConcealedKan:
        if (!node->start().isTerminal()) {
          return false;
        }
        break;
    }
  }
    for (const auto& meld : player.melds_range()) {
    switch (meld.type) {
      case SetType::kSingle:
        return false;
      case SetType::kChi:
        if (!meld.start.isTerminal() && !(meld.start + 2).isTerminal()) {
          return false;
        }
        break;
      case SetType::kPair:
      case SetType::kKan:
      case SetType::kConcealedKan:
      case SetType::kPon:
        if (!meld.start.isTerminal()) {
          return false;
        }
        break;
    }
  }
  return true;
}

REGISTER_YAKU({
    .id = "terminalsinallsets",
    .name = "Terminals in all Sets",
    .type = Yaku::kBonusWhenClosed,
    .value = 2,
    .is_yaku_func = yaku::isTerminalsInAllSets,
});
}  // namespace mahjong::yaku
