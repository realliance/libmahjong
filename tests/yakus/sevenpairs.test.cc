#include "scoring/yakus/sevenpairs.h"
#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "utils/handformer.h"

namespace mahjong::yakus {

TEST(isSevenPairs, 2Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m22p33s44z55m11z66z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSevenPairs(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isSevenPairs, MustBeConcealed) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m22p33s44z55m11z66z"));
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSevenPairs(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isSevenPairs, UniquePairsOnly) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("1111m22p33s44z11z66z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSevenPairs(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yakus
