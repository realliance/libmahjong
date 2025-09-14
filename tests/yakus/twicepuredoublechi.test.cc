#include "scoring/yakus/twicepuredoublechi.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isTwicePureDoubleChi, 3Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("789p789p234m234m11z", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTwicePureDoubleChi(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTwicePureDoubleChi, MustBeConcealed) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("789p789p234m234m11z", &hand);
  hand.open = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTwicePureDoubleChi(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isTwicePureDoubleChi, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("789p789p234m567m11z", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTwicePureDoubleChi(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
