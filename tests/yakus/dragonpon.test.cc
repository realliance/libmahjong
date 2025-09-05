#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/honorpon.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isWindOrDragonPon, WhiteDragon) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("123m456m555z11z", &player);

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isWhiteDragon(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, GreenDragon) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("123m456m666z11z", &player);

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isGreenDragon(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, RedDragon) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("123m456m777z11z", &player);

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRedDragon(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Kan) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("123m456m7777z11z", &player);

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRedDragon(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, CanWhenOpen) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("123m456m777z11z", &player);
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRedDragon(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, BadPlayer) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("123m456m222p11p", &player);

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isGreenDragon(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
