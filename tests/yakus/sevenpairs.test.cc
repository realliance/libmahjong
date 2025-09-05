#include "scoring/yakus/sevenpairs.h"

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

TEST(isSevenPairs, 2Han) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("11m22p33s44z55m11z66z", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSevenPairs(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isSevenPairs, MustBeConcealed) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("11m22p33s44z55m11z66z", &player);
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSevenPairs(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isSevenPairs, UniquePairsOnly) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("1111m22p33s44z11z66z", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isSevenPairs(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
