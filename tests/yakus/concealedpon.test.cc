#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/threeconcealedpons.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {
TEST(isThreeConcealedPons, 2Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m111p111s666z44m", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealed) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m111p111s44m", &player);
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kChi,
      .start = kTwoBamboo,
  };
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, CanHaveAdditionalOpenPon) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m111p111s44m", &player);
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = kTwoBamboo,
  };
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealedNegative) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m111p234s44m", &player);
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = kOneBamboo,
  };
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeConcealedPons, BadPlayer) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m111p234567s44m", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
