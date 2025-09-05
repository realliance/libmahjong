#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/allpons.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isAllPons, 2Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m222p888s666z44m", &player);

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, WithKans) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m222p666z44m", &player);
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kKan,
      .start = Piece(kFivePin),
  };
  player.open = true;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, ConcealedKan) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m222p666z44m", &player);
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kConcealedKan,
      .start = Piece(kFivePin),
  };
  player.open = false;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, BadPlayer) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m222p666z789m44m", &player);
  player.open = false;

  auto root = breakdownPlayer(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
