#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"
#include "analysis/hands.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(isAllSimples, 1Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("222m333p444s555p88m"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllSimples(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllSimples, BadHandHonors) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("222m333p444s111z88m"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllSimples(game_state, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isAllSimples, BadHandTerminals) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("222m333p444s111m88m"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllSimples(game_state, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
