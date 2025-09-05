#include "scoring/yakus/allterminalsandhonors.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isAllTerminalsAndHonors, 2Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("222z111p111m999s66z", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllTerminalsAndHonors(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllTerminalsAndHonors, BadPlayer) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("222z111p111m888s66z", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllTerminalsAndHonors(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isAllTerminalsAndHonors, CanBeOpen) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("222z111m999s66z", &player);
  player.open = true;
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kNinePin),
  };

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllTerminalsAndHonors(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}
}  // namespace mahjong::yaku
