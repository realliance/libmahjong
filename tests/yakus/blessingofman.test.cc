#include "scoring/yakus/blessingofman.h"

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
TEST(isBlessingOfMan, 5Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[3];
  HandFromNotation("123m123p444m111z55m", &hand);
  hand.open = false;
  hand.id = 3;

  game_state.turnNum = 1;

  // No calls have occured
  game_state.lastCall = -1;

  hand.hasRonned = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBlessingOfMan, MustBeARon) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[3];
  HandFromNotation("123m123p444m111z55m", &hand);
  hand.open = false;

  game_state.turnNum = 3;

  // No calls have occured
  game_state.lastCall = -1;

  hand.hasRonned = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfMan, MustBeBeforePlayerFirstTurn) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123m123p444m111z55m", &hand);
  hand.open = false;

  game_state.turnNum = 2;

  // No calls have occured
  game_state.lastCall = -1;

  hand.hasRonned = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfMan, NoCalledMustHaveOccured) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[3];
  HandFromNotation("123m123p444m111z55m", &hand);
  hand.open = false;

  game_state.turnNum = 1;

  game_state.lastCall = 0;

  hand.hasRonned = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
