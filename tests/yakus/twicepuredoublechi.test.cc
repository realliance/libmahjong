#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "scoring/yakus.h"

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
    if (mahjong::isTwicePureDoubleChi(game_state, 0, branch)) {
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
    if (mahjong::isTwicePureDoubleChi(game_state, 0, branch)) {
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
    if (mahjong::isTwicePureDoubleChi(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
