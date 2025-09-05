#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/terminalsinallsets.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isTerminalsInAllSets, 2Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m111p11s", &player);
  player.open = true;
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kChi,
      .start = Piece(kSevenPin),
  };

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTerminalsInAllSets(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, 3Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m111p789p11s", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTerminalsInAllSets(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, BadPlayer) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("123m789m222p11s", &player);
  player.open = true;
  player.open = true;
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kChi,
      .start = Piece(kSevenPin),
  };

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTerminalsInAllSets(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
