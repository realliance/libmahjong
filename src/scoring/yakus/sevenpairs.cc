#include "scoring/yakus/sevenpairs.h"

#include <set>
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/piecetype.h"

namespace mahjong::yaku {
bool isSevenPairs(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& branch) {
  if (state.hands[player].open) {
    return false;
  }
  std::set<Piece> pairs;
  for (const auto& node : branch) {
    switch (node->type()) {
      case Node::kError:
      case Node::kChiSet:
      case Node::kPonSet:
      case Node::kSingle:
        return false;
      case Node::kPair:
        pairs.insert(node->start());
      case Node::kRoot:
        break;
    }
  }
  return pairs.size() == 7;
}

}  // namespace mahjong::yaku
