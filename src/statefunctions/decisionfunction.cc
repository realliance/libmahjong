#include "statefunctions/decisionfunction.h"

#include <algorithm>
#include <array>

#include "analysis/util.h"
#include "scoring/scoring.h"
#include "scoring/yakus/thirteenorphans.h"
#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/walls.h"

namespace mahjong {

// TODO(#18): "I really hate this" - alice
bool CanRon(const GameState& state, const Hand& hand) {
  // If the pending piece is your discard, you can't Ron
  for (const auto& piece : hand.discards) {
    if (state.pendingPiece == piece) {
      return false;
    }
  }

  // Build the theoretical hand
  Hand tmp_hand = hand;
  tmp_hand.live[tmp_hand.live_count++] = state.pendingPiece;

  // If this Ron is occurring due to a concealed kan discard,
  if (state.concealedKan) {
    // If it happens to be a ron for a thirteen orphans,
    // it's allowed and you can ron otherwise, you can't.
    return yaku::isThirteenOrphans(state, hand);
  }
  // if not a concealed kan, check if it's complete
  return isComplete(state, hand);
}

bool CanKan(const GameState& state, const Hand& hand) {
  if (Walls::GetRemainingPieces(state) == 0) {
    return false;
  }
  if (hand.riichi) {
    return false;
  }
  return CountPieces(hand, state.pendingPiece) == 3;
}

bool CanPon(const GameState& state, const Hand& hand) {
  if (hand.riichi) {
    return false;
  }
  return CountPieces(hand, state.pendingPiece) == 2;
}

bool CanChi(const GameState& state, const Hand& hand) {
  if (hand.riichi) {
    return false;
  }
  if (state.pendingPiece.isHonor()) {
    return false;
  }
  if (((state.currentPlayer + 1) % 4) != hand.id) {
    return false;
  }
  if (CountPieces(hand, state.pendingPiece - 2) > 0 &&
      CountPieces(hand, state.pendingPiece - 1) > 0) {
    return true;
  }
  if (CountPieces(hand, state.pendingPiece - 1) > 0 &&
      CountPieces(hand, state.pendingPiece + 1) > 0) {
    return true;
  }
  if (CountPieces(hand, state.pendingPiece + 1) > 0 &&
      CountPieces(hand, state.pendingPiece + 2) > 0) {
    return true;
  }
  return false;
}

bool CanTsumo(const GameState& state, const Hand& hand) {
  return isComplete(state, hand);
}

bool CanConvertedKan(const GameState& state, const Hand& hand) {
  if (Walls::GetRemainingPieces(state) == 0) {
    return false;
  }
  return std::any_of(
      hand.melds.begin(), hand.melds.begin() + hand.meld_count, [&](auto meld) {
        return meld.type == SetType::kPon && CountPieces(hand, meld.start) == 1;
      });
}

bool CanConcealedKan(const GameState& state, const Hand& hand) {
  if (Walls::GetRemainingPieces(state) == 0) {
    return false;
  }
  // TODO(#19): Allow riichi concealed kan under the right conditions
  if (hand.riichi) {
    return false;
  }
  return CountPieces(hand, state.pendingPiece) == 4;
}

bool CanRiichi(const GameState& /*unused*/, const Hand& hand) {
  if (hand.riichi) {
    return false;
  }
  if (hand.open) {
    return false;
  }
  return !getPossibleWaits(hand).empty();
}
}  // namespace mahjong
