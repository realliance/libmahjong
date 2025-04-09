#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis.h"
#include "gamestate.h"
#include "hand.h"
#include "handformer.h"
#include "handnode.h"
#include "hands.h"
#include "piecetype.h"
#include "statefunctions.h"

namespace mahjong {

TEST(isAfterAKan, 1Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m1111z999s55z"));
  game_state.currentPlayer = 0;

  game_state.prevState = mahjong::Replacement;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAfterAKan(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAfterAKan, DoesntApply) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m1111z999s55z"));
  game_state.currentPlayer = 0;

  game_state.prevState = mahjong::Pon;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAfterAKan(game_state, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isAfterAKan, WrongPlayer) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m1111z999s55z"));
  game_state.currentPlayer = 2;

  game_state.prevState = mahjong::Replacement;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAfterAKan(game_state, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
