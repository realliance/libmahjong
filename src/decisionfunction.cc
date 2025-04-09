#include "decisionfunction.h"

#include <algorithm>
#include <array>
#include <vector>

#include "gamestate.h"
#include "hand.h"
#include "hands.h"
#include "meld.h"
#include "piecetype.h"
#include "stateutilities.h"
#include "walls.h"

auto mahjong::CanRon(const GameState& state, int player)
    -> bool {  // TODO(alice): I really hate this
  for (const auto& piece : state.hands.at(player).discards) {
    if (state.pendingPiece == piece) {
      return false;
    }
  }
  auto& tmp_state = const_cast<GameState&>(state);
  tmp_state.hands.at(player).live.push_back(state.pendingPiece);
  tmp_state.hands.at(player).sort();
  if (state.concealedKan) {
    if (isThirteenOrphans(state, player) != 0) {
      tmp_state.hands.at(player).live.erase(
          std::find(state.hands.at(player).live.begin(),
                    state.hands.at(player).live.end(), state.pendingPiece));
      return true;
    }
    tmp_state.hands.at(player).live.erase(
        std::find(state.hands.at(player).live.begin(),
                  state.hands.at(player).live.end(), state.pendingPiece));
    return false;
  }
  bool can_ron = isComplete(state, player);
  tmp_state.hands.at(player).live.erase(
      std::find(state.hands.at(player).live.begin(),
                state.hands.at(player).live.end(), state.pendingPiece));
  return can_ron;
}

auto mahjong::CanKan(const GameState& state, int player) -> bool {
  if (state.walls.GetRemainingPieces() == 0) {
    return false;
  }
  if (state.hands.at(player).riichi) {
    return false;
  }
  return CountPieces(state, player, state.pendingPiece) == 3;
}

auto mahjong::CanPon(const GameState& state, int player) -> bool {
  if (state.hands.at(player).riichi) {
    return false;
  }
  return CountPieces(state, player, state.pendingPiece) == 2;
}

auto mahjong::CanChi(const GameState& state, int player) -> bool {
  if (state.hands.at(player).riichi) {
    return false;
  }
  if (state.pendingPiece.isHonor()) {
    return false;
  }
  if (((state.currentPlayer + 1) % 4) != player) {
    return false;
  }
  if (CountPieces(state, player, state.pendingPiece - 2) > 0 &&
      CountPieces(state, player, state.pendingPiece - 1) > 0) {
    return true;
  }
  if (CountPieces(state, player, state.pendingPiece - 1) > 0 &&
      CountPieces(state, player, state.pendingPiece + 1) > 0) {
    return true;
  }
  if (CountPieces(state, player, state.pendingPiece + 1) > 0 &&
      CountPieces(state, player, state.pendingPiece + 2) > 0) {
    return true;
  }
  return false;
}

auto mahjong::CanTsumo(const GameState& state) -> bool {
  return isComplete(state, state.currentPlayer);
}

auto mahjong::CanConvertedKan(const GameState& state) -> bool {
  if (state.walls.GetRemainingPieces() == 0) {
    return false;
  }
  return std::any_of(
      state.hands.at(state.currentPlayer).melds.begin(),
      state.hands.at(state.currentPlayer).melds.end(), [&](auto meld) {
        return meld.type == Meld::kPon &&
               CountPieces(state, state.currentPlayer, meld.start) == 1;
      });
}

auto mahjong::CanConcealedKan(const GameState& state) -> bool {
  if (state.walls.GetRemainingPieces() == 0) {
    return false;
  }
  if (state.hands.at(state.currentPlayer).riichi) {
    return false;  // arguably this should be allowed sometimes
  }
  return CountPieces(state, state.currentPlayer, state.pendingPiece) == 4;
}

auto mahjong::CanRiichi(const GameState& state) -> bool {
  if (state.hands.at(state.currentPlayer).riichi) {
    return false;
  }
  if (state.hands.at(state.currentPlayer).open) {
    return false;
  }
  return !isInTenpai(state.hands.at(state.currentPlayer).live).empty();
}
