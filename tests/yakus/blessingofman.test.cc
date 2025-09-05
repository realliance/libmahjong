#include "scoring/yakus/blessingofman.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "utils/handformer.h"

namespace mahjong::yaku {
TEST(isBlessingOfMan, 5Han) {
  auto game_state = GameState();
  Player& player =game_state.players[3];
  HandFromNotation("123m123p444m111z55m", &player);
  player.open = false;
  player.id = 3;

  game_state.turnNum = 1;

  // No calls have occured
  game_state.lastCall = -1;

  player.hasRonned = true;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBlessingOfMan, MustBeARon) {
  auto game_state = GameState();
  Player& player =game_state.players[3];
  HandFromNotation("123m123p444m111z55m", &player);
  player.open = false;

  game_state.turnNum = 3;

  // No calls have occured
  game_state.lastCall = -1;

  player.hasRonned = false;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfMan, MustBeBeforePlayerFirstTurn) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m123p444m111z55m", &player);
  player.open = false;

  game_state.turnNum = 2;

  // No calls have occured
  game_state.lastCall = -1;

  player.hasRonned = true;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfMan, NoCalledMustHaveOccured) {
  auto game_state = GameState();
  Player& player =game_state.players[3];
  HandFromNotation("123m123p444m111z55m", &player);
  player.open = false;

  game_state.turnNum = 1;

  game_state.lastCall = 0;

  player.hasRonned = true;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isBlessingOfMan(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
