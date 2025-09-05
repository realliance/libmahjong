#include <gtest/gtest.h>

#include <array>
#include <memory>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/bottomofthesea.h"
#include "types/gamestate.h"
#include "types/settings.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isBottomOfTheSea, 1Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m111z999s55z", &player);
  game_state.livingWallIndex = kLivingWallCount;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBottomOfTheSea(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBottomOfTheSea, 1HanRonned) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m111z999s55z", &player);
  game_state.livingWallIndex = kLivingWallCount;
  game_state.players[0].hasRonned = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBottomOfTheSea(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBottomOfTheSea, DoesntApply) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m111z999s55z", &player);

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBottomOfTheSea(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
