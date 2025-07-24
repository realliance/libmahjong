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

TEST(isPureStraight, DISABLED_Open) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m789m555p11z"));
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isPureStraight(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPureStraight, DISABLED_Closed) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m789m555p11z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isPureStraight(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPureStraight, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m999m555p11z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isPureStraight(game_state, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
