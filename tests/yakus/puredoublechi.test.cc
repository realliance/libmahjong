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
  Player& player =game_state.players[0];
  HandFromNotation("234m234m555p888s88p", &player);
  player.open = false;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPureDoubleChi(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPureDoubleChi, BadPlayer) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("234m123m555p888s88p", &player);
  player.open = false;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPureDoubleChi(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPureDoubleChi, MustBeConcealed) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("234m234m555p888s88p", &player);
  player.open = true;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPureDoubleChi(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
