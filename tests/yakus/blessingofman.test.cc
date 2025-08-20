#include "scoring/yakus/blessingofman.h"
#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "utils/handformer.h"

namespace mahjong::yaku {
TEST(isBlessingOfMan, 5Han) {
  auto game_state = GameState();
  game_state.hands[3] = Hand(HandFromNotation("123m123p444m111z55m"));
  game_state.hands[3].open = false;

  game_state.turnNum = 1;

  // No calls have occured
  game_state.lastCall = -1;

  game_state.hasRonned[3] = true;

  auto root = breakdownHand(game_state.hands.at(3).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, 3, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBlessingOfMan, MustBeARon) {
  auto game_state = GameState();
  game_state.hands[3] = Hand(HandFromNotation("123m123p444m111z55m"));
  game_state.hands[3].open = false;

  game_state.turnNum = 3;

  // No calls have occured
  game_state.lastCall = -1;

  game_state.hasRonned[3] = false;

  auto root = breakdownHand(game_state.hands.at(3).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, 3, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfMan, MustBeBeforePlayerFirstTurn) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m123p444m111z55m"));
  game_state.hands[0].open = false;

  game_state.turnNum = 2;

  // No calls have occured
  game_state.lastCall = -1;

  game_state.hasRonned[0] = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfMan, NoCalledMustHaveOccured) {
  auto game_state = GameState();
  game_state.hands[3] = Hand(HandFromNotation("123m123p444m111z55m"));
  game_state.hands[3].open = false;

  game_state.turnNum = 1;

  game_state.lastCall = 0;

  game_state.hasRonned[3] = true;

  auto root = breakdownHand(game_state.hands.at(3).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, 3, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
