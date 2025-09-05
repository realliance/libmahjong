#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/robbingakan.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/statefunction.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isRobbingAKan, 1Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m1111z999s55z", &player);
  game_state.players[0].hasRonned = true;
  game_state.nextState = StateFunctionType::kKanDiscard;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRobbingAKan(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isRobbingAKan, DoesntApply) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m1111z999s55z", &player);
  game_state.players[0].hasRonned = true;
  game_state.nextState = StateFunctionType::kPon;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isRobbingAKan(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
