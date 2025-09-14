#include "scoring/yakus/allsimples.h"

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

TEST(isAllSimples, 1Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("222m333p444s555p88m", &hand);

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllSimples(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllSimples, BadHandHonors) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("222m333p444s111z88m", &hand);

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllSimples(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isAllSimples, BadHandTerminals) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("222m333p444s111m88m", &hand);

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllSimples(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
