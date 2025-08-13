#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"

#include "scoring/yakus/bottomofthesea.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/walls.h"
#include "utils/handformer.h"

namespace mahjong::yakus {

TEST(isBottomOfTheSea, 1Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m111z999s55z"));
  game_state.walls.livingWalls.clear();

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBottomOfTheSea(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBottomOfTheSea, 1HanRonned) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m111z999s55z"));
  game_state.walls.livingWalls.clear();
  game_state.hasRonned[0] = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBottomOfTheSea(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBottomOfTheSea, DoesntApply) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m111z999s55z"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBottomOfTheSea(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yakus
