#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "scoring/yakus.h"

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/statefunction.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(isRobbingAKan, 1Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m1111z999s55z"));
  game_state.hasRonned[0] = true;
  game_state.nextState = StateFunctionType::kKanDiscard;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRobbingAKan(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isRobbingAKan, DoesntApply) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m1111z999s55z"));
  game_state.hasRonned[0] = true;
  game_state.nextState = StateFunctionType::kPon;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRobbingAKan(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
