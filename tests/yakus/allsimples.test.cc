#include "scoring/yakus/allsimples.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isAllSimples, 1Han) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("222m333p444s555p88m");

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllSimples(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllSimples, BadHandHonors) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("222m333p444s111z88m");

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllSimples(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isAllSimples, BadHandTerminals) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("222m333p444s111m88m");

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllSimples(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
