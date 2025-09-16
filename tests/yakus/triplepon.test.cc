#include "scoring/yakus/triplepon.h"

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

TEST(isTriplePon, 2Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m111p111s666z44m"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTriplePon(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTriplePon, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m111p222m666z44m"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTriplePon(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
