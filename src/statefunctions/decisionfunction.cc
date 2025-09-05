#include "statefunctions/decisionfunction.h"

#include <algorithm>
#include <array>

#include "analysis/util.h"
#include "scoring/scoring.h"
#include "scoring/yakus/thirteenorphans.h"
#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/walls.h"

namespace mahjong {

// TODO(#18): "I really hate this" - alice
bool CanRon(const GameState& state, const Player& player) {
  // If the pending piece is your discard, you can't Ron
  for (const auto& piece : player.discards) {
    if (state.pendingPiece == piece) {
      return false;
    }
  }

  // Build the theoretical hand
  auto& tmp_player = const_cast<Player&>(player);
  tmp_player.live[tmp_player.live_count++] = state.pendingPiece;

  // If this Ron is occurring due to a concealed kan discard,
  if (state.concealedKan) {
    // If it happens to be a ron for a thirteen orphans,
    // it's allowed and you can ron
    if (yaku::isThirteenOrphans(state, player)) {
      tmp_player.live[--tmp_player.live_count] = kError;
      return true;
    }

    // otherwise, you can't
    tmp_player.live[--tmp_player.live_count] = kError;
    return false;
  }

  // if not a concealed kan, check if it's complete
  const bool can_ron = isComplete(state, player);
  tmp_player.live[--tmp_player.live_count] = kError;
  return can_ron;
}

bool CanKan(const GameState& state, const Player& player) {
  if (Walls::GetRemainingPieces(state) == 0) {
    return false;
  }
  if (player.riichi) {
    return false;
  }
  return CountPieces(player, state.pendingPiece) == 3;
}

bool CanPon(const GameState& state, const Player& player) {
  if (player.riichi) {
    return false;
  }
  return CountPieces(player, state.pendingPiece) == 2;
}

bool CanChi(const GameState& state, const Player& player) {
  if (player.riichi) {
    return false;
  }
  if (state.pendingPiece.isHonor()) {
    return false;
  }
  if (((state.currentPlayer + 1) % 4) != player.id) {
    return false;
  }
  if (CountPieces(player, state.pendingPiece - 2) > 0 &&
      CountPieces(player, state.pendingPiece - 1) > 0) {
    return true;
  }
  if (CountPieces(player, state.pendingPiece - 1) > 0 &&
      CountPieces(player, state.pendingPiece + 1) > 0) {
    return true;
  }
  if (CountPieces(player, state.pendingPiece + 1) > 0 &&
      CountPieces(player, state.pendingPiece + 2) > 0) {
    return true;
  }
  return false;
}

bool CanTsumo(const GameState& state, const Player& player) {
  return isComplete(state, player);
}

bool CanConvertedKan(const GameState& state, const Player& player) {
  if (Walls::GetRemainingPieces(state) == 0) {
    return false;
  }
  return std::any_of(player.melds.begin(),
                     player.melds.begin() + player.meld_count, [&](auto meld) {
                       return meld.type == SetType::kPon &&
                              CountPieces(player, meld.start) == 1;
                     });
}

bool CanConcealedKan(const GameState& state, const Player& player) {
  if (Walls::GetRemainingPieces(state) == 0) {
    return false;
  }
  // TODO(#19): Allow riichi concealed kan under the right conditions
  if (player.riichi) {
    return false;
  }
  return CountPieces(player, state.pendingPiece) == 4;
}

bool CanRiichi(const GameState& /*unused*/, const Player& player) {
  if (player.riichi) {
    return false;
  }
  if (player.open) {
    return false;
  }
  return !getPossibleWaits(player).empty();
}
}  // namespace mahjong
