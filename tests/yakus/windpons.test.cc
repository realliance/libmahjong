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

TEST(isWindOrDragonPon, SeatWind) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m444z55z"));
  game_state.roundNum = 1;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isWindOrDragonPon(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, SeatWindKan) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m4444z55z"));
  game_state.roundNum = 1;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isWindOrDragonPon(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, PrevalentWind) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m111z55z"));
  game_state.roundNum = 1;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isWindOrDragonPon(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, PrevalentWindKan) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m1111z55z"));
  game_state.roundNum = 1;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isWindOrDragonPon(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Dealer2Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m111z55z"));
  game_state.roundNum = 0;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isWindOrDragonPon(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Dealer2HanKan) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m1111z55z"));
  game_state.roundNum = 0;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isWindOrDragonPon(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, NotSeatOrPrevalent) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m222z55z"));
  game_state.roundNum = 0;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isWindOrDragonPon(game_state, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isWindOrDragonPon, NoWind) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456m666m55z"));
  game_state.roundNum = 0;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isWindOrDragonPon(game_state, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

}  // namespace mahjong
