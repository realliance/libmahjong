#include "scoring/yakus/halfflush.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isHalfFlush, 2Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("11m234m111z222z", &hand);
  hand.open = true;
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, 3Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("11m234m777m111z222z", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("11m234m111p222z", &hand);
  hand.open = true;
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isHalfFlush, FullFlushIncompatible) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("11m234m888m777m", &hand);
  hand.open = true;
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
