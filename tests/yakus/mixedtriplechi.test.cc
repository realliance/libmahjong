#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/yakus.h"

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(isMixedTripleChi, Open) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123p123m123s555p11z"));
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isMixedTripleChi(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isMixedTripleChi, Closed) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123p123m123s555p11z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isMixedTripleChi(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isMixedTripleChi, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123p123p123s555p11z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isMixedTripleChi(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
