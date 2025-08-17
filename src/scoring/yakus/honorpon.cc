#include "scoring/yakus/honorpon.h"

#include <vector>

#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong::yaku {
namespace {
bool findPon(const GameState& state, int player,
             const std::vector<const mahjong::Node*>& branch,
             const Piece& piece) {
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet && node->start() == piece) {
      return true;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kPon && meld.start == piece) {
      return true;
    }
  }
  return false;
}

}  // namespace

bool isSeatWind(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch,
                 Piece::fromWind(GetSeat(state.roundNum, player)));
}

bool isPrevalentWind(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch,
                 Piece::fromWind(state.roundNum > 3 ? kSouth : kEast));
}

bool isGreenDragon(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kGreenDragon);
}

bool isRedDragon(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kRedDragon);
}

bool isWhiteDragon(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kWhiteDragon);
}

}  // namespace mahjong::yaku
