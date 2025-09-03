#include "scoring/yakus/mixedtriplechi.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isMixedTripleChi, Open) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123p123m123s555p11z", &hand);
  hand.open = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isMixedTripleChi(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isMixedTripleChi, Closed) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123p123m123s555p11z", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isMixedTripleChi(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isMixedTripleChi, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123p123p123s555p11z", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isMixedTripleChi(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
