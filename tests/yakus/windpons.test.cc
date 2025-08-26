#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/honorpon.h"
#include "types/gamestate.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isWindOrDragonPon, SeatWind) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m456m444z55z");
  game_state.roundNum = 1;

  auto root = breakdownHand(game_state.hands.at(0).live);

  auto branches = Node::AsBranchVectors(root.get());

  for (const auto& branch : branches) {
    if (isSeatWind(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }

  FAIL();
}

TEST(isWindOrDragonPon, SeatWindKan) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m456m4444z55z");
  game_state.roundNum = 1;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSeatWind(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, PrevalentWind) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m456m111z55z");
  game_state.roundNum = 1;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPrevalentWind(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, PrevalentWindKan) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m456m1111z55z");
  game_state.roundNum = 1;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPrevalentWind(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Dealer2Han) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m456m111z55z");
  game_state.roundNum = 0;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSeatWind(game_state, 0, branch) &&
        isPrevalentWind(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Dealer2HanKan) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m456m1111z55z");
  game_state.roundNum = 0;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSeatWind(game_state, 0, branch) &&
        isPrevalentWind(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, NotSeatOrPrevalent) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m456m222z55z");
  game_state.roundNum = 0;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSeatWind(game_state, 0, branch) ||
        isPrevalentWind(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isWindOrDragonPon, NoWind) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m456m666m55z");
  game_state.roundNum = 0;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSeatWind(game_state, 0, branch) ||
        isPrevalentWind(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

}  // namespace mahjong::yaku
