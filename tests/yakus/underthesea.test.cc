#include <gtest/gtest.h>

#include <array>
#include <memory>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/bottomofthesea.h"
#include "types/gamestate.h"
#include "types/settings.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isBottomOfTheSea, 1Han) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("123m789m111z999s55z");
  game_state.livingWallIndex = kLivingWallCount;

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
  game_state.hands[0].live = HandFromNotation("123m789m111z999s55z");
  game_state.livingWallIndex = kLivingWallCount;
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
  game_state.hands[0].live = HandFromNotation("123m789m111z999s55z");

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBottomOfTheSea(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
