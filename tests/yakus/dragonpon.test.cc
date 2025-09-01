#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"

#include "analysis/handnode.h"
#include "scoring/yakus/honorpon.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isWindOrDragonPon, WhiteDragon) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m555z11z"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isWhiteDragon(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, GreenDragon) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m666z11z"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isGreenDragon(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, RedDragon) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m777z11z"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRedDragon(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Kan) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m7777z11z"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRedDragon(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, CanWhenOpen) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m777z11z"));
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRedDragon(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m222p11p"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isGreenDragon(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
