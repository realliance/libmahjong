#include "scoring/yakus/outsidehand.h"

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

TEST(isOutsideHand, Open) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("123m789m111z999s55z", &player);
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isOutsideHand(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isOutsideHand, Closed) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("123m789m111z999s55z", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isOutsideHand(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isOutsideHand, NoChi) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("111m111p111s999m66z", &player);

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isOutsideHand(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isOutsideHand, BadPlayer) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("111m234p111s999m66z", &player);

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isOutsideHand(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
