#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/hands.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(isTwicePureDoubleChi, 3Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("789p789p234m234m11z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTwicePureDoubleChi(game_state, 0, branch) == 3) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTwicePureDoubleChi, MustBeConcealed) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("789p789p234m234m11z"));
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTwicePureDoubleChi(game_state, 0, branch) == 3) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isTwicePureDoubleChi, PureDoubleChowIncompatible) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("789p789p234m234m11z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTwicePureDoubleChi(game_state, 0, branch) == 3 &&
        mahjong::isPureDoubleChi(game_state, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isTwicePureDoubleChi, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("789p789p234m567m11z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTwicePureDoubleChi(game_state, 0, branch) == 3) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
