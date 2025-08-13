#include "scoring/yakus/ninegates.h"

#include <map>
#include <vector>

#include "scoring/yakus/fullflush.h"
#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isNineGates(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (!isFullFlush(state, player, branch)) {
    return false;
  }
  if (state.hands.at(player).open) {
    return false;
  }
  std::map<int, int> pieces;
  for (int i = 1; i < 10; i++) {
    pieces.at(i) = 0;
  }
  bool duplicate = false;
  for (const auto& piece : state.hands.at(player).live) {
    if (pieces.contains(piece.getPieceNum())) {
      if ((pieces[piece.getPieceNum()] != 0) && !piece.isTerminal()) {
        if (duplicate) {
          return false;
        }
        duplicate = true;
      } else {
        pieces[piece.getPieceNum()]++;
      }
    }
  }
  if (!duplicate) {
    return false;
  }
  for (const auto& [piece, count] : pieces) {
    if ((piece == 1 || piece == 9)) {
      if (count != 3) {
        return false;
      }
    } else if (count != 1) {
      return false;
    }
  }
  return true;
}

}  // namespace mahjong::yaku
