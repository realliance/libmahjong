#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"
#include "utils/handformer.h"
#include "analysis/hands.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/piecetype.h"

namespace mahjong {

TEST(isOutsideHand, Open) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m111z999s55z"));
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isOutsideHand(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isOutsideHand, Closed) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m111z999s55z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isOutsideHand(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isOutsideHand, NoChi) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m111p111s999m66z"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isOutsideHand(game_state, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isOutsideHand, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m234p111s999m66z"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isOutsideHand(game_state, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
