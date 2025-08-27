#include "scoring/yakus/puredoublechi.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isPureDoubleChi, 1Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  hand.live = HandFromNotation("234m234m555p888s88p");
  hand.open = false;

  auto root = breakdownHand(hand.live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPureDoubleChi(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPureDoubleChi, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  hand.live = HandFromNotation("234m123m555p888s88p");
  hand.open = false;

  auto root = breakdownHand(hand.live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPureDoubleChi(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPureDoubleChi, MustBeConcealed) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  hand.live = HandFromNotation("234m234m555p888s88p");
  hand.open = true;

  auto root = breakdownHand(hand.live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPureDoubleChi(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
