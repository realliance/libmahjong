#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/afterakan.h"
#include "types/gamestate.h"
#include "types/statefunction.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isAfterAKan, 1Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m1111z999s55z", &player);
  game_state.currentPlayer = 0;

  game_state.prevState = StateFunctionType::kReplacement;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAfterAKan(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAfterAKan, DoesntApply) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m1111z999s55z", &player);
  game_state.currentPlayer = 0;

  game_state.prevState = StateFunctionType::kPon;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAfterAKan(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isAfterAKan, WrongPlayer) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m1111z999s55z", &player);
  game_state.currentPlayer = 2;

  game_state.prevState = StateFunctionType::kReplacement;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAfterAKan(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
