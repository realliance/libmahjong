#include "scoring/yakus/twicepuredoublechi.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isTwicePureDoubleChi, 3Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("789p789p234m234m11z", &player);
  player.open = false;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTwicePureDoubleChi(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTwicePureDoubleChi, MustBeConcealed) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("789p789p234m234m11z", &player);
  player.open = true;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTwicePureDoubleChi(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isTwicePureDoubleChi, BadPlayer) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("789p789p234m567m11z", &player);
  player.open = false;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTwicePureDoubleChi(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
