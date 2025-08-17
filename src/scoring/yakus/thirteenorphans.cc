#include "scoring/yakus/thirteenorphans.h"

#include <map>
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong::yaku {
bool isThirteenOrphans(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  std::map<Piece, bool> pieces = {
      {kOneCharacter, false}, {kNineCharacter, false}, {kOnePin, false},
      {kNinePin, false},      {kOneBamboo, false},     {kNineBamboo, false},
      {kEastWind, false},     {kSouthWind, false},     {kWestWind, false},
      {kNorthWind, false},    {kRedDragon, false},     {kWhiteDragon, false},
      {kGreenDragon, false}};
  bool duplicate = false;
  for (const auto& piece : state.hands.at(player).live) {
    if (pieces.contains(piece)) {
      if (pieces[piece]) {
        duplicate = true;
      } else {
        pieces[piece] = true;
      }
    }
  }
  if (!duplicate) {
    return false;
  }
  for (const auto& [_, match] : pieces) {
    if (!match) {
      return false;
    }
  }
  return true;
}

}  // namespace mahjong::yaku
