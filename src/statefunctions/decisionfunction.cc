#include "decisionfunction.h"

#include <algorithm>
#include <array>
#include <vector>

#include "analysis/hands.h"
#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/walls.h"

namespace mahjong {

// TODO(#18): "I really hate this" - alice
bool CanRon(const GameState& state, int player) {

  // If the pending piece is your disables, you can't Ron
  for (const auto& piece : state.hands.at(player).discards) {
    if (state.pendingPiece == piece) {
      return false;
    }
  }

  // Build the theoretical hand
  auto& tmp_state = const_cast<GameState&>(state);
  tmp_state.hands.at(player).live.push_back(state.pendingPiece);
  tmp_state.hands.at(player).sort();

  // If this Ron is occurring due to a concealed kan discard,
  if (state.concealedKan) {
    // If it happens to be a ron for a thirteen orphans,
    // it's allowed and you can ron
    if (isThirteenOrphans(state, player) != 0) {
      tmp_state.hands.at(player).live.erase(
          std::find(state.hands.at(player).live.begin(),
                    state.hands.at(player).live.end(), state.pendingPiece));
      return true;
    }

    // otherwise, you can't
    tmp_state.hands.at(player).live.erase(
        std::find(state.hands.at(player).live.begin(),
                  state.hands.at(player).live.end(), state.pendingPiece));
    return false;
  }

  // if not a concealed kan, check if it's complete
  const bool can_ron = isComplete(state, player);
  tmp_state.hands.at(player).live.erase(
      std::find(state.hands.at(player).live.begin(),
                state.hands.at(player).live.end(), state.pendingPiece));
  return can_ron;
}

bool CanKan(const GameState& state, int player) {
  if (state.walls.GetRemainingPieces() == 0) {
    return false;
  }
  if (state.hands.at(player).riichi) {
    return false;
  }
  return CountPieces(state, player, state.pendingPiece) == 3;
}

bool CanPon(const GameState& state, int player) {
  if (state.hands.at(player).riichi) {
    return false;
  }
  return CountPieces(state, player, state.pendingPiece) == 2;
}

bool CanChi(const GameState& state, int player) {
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

bool CanTsumo(const GameState& state, int player) {
  return isComplete(state, player);
}

bool CanConvertedKan(const GameState& state, int player) {
  if (state.walls.GetRemainingPieces() == 0) {
    return false;
  }
  return std::any_of(state.hands.at(player).melds.begin(),
                     state.hands.at(player).melds.end(), [&](auto meld) {
                       return meld.type == Meld::kPon &&
                              CountPieces(state, player, meld.start) == 1;
                     });
}

bool CanConcealedKan(const GameState& state, int player) {
  if (state.walls.GetRemainingPieces() == 0) {
    return false;
  }
  // TODO(#19): Allow riichi concealed kan under the right conditions
  if (state.hands.at(player).riichi) {
    return false;
  }
  return CountPieces(state, player, state.pendingPiece) == 4;
}

bool CanRiichi(const GameState& state, int player) {
  if (state.hands.at(player).riichi) {
    return false;
  }
  if (state.hands.at(player).open) {
    return false;
  }
  return !isInTenpai(state.hands.at(player).live).empty();
}
}  // namespace mahjong
