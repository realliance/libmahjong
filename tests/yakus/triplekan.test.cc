#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/threekans.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isThreeKans, 2Han) {
  const Meld meld_a = {
      .type = SetType::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = SetType::kKan,
      .start = Piece(kTwoBamboo),
  };
  const Meld meld_c = {
      .type = SetType::kKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("666z22m", &player);
  player.melds = {meld_a, meld_b, meld_c};
  player.meld_count = 3;
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeKans(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, WithOtherMelds) {
  const Meld meld_a = {
      .type = SetType::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = SetType::kKan,
      .start = Piece(kTwoBamboo),
  };
  const Meld meld_c = {
      .type = SetType::kKan,
      .start = Piece(kFivePin),
  };
  const Meld meld_d = {
      .type = SetType::kChi,
      .start = Piece(kTwoBamboo),
  };

  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("22m", &player);
  player.melds = {meld_a, meld_b, meld_c, meld_d};
  player.meld_count = 4;
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeKans(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, BadPlayer) {
  const Meld meld_a = {
      .type = SetType::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = SetType::kKan,
      .start = Piece(kTwoBamboo),
  };

  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("777p666z22m", &player);
  player.melds = {meld_a, meld_b};
  player.meld_count = 2;
  player.open = true;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeKans(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeKans, AllConcealed) {
  const Meld meld_a = {
      .type = SetType::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = SetType::kConcealedKan,
      .start = Piece(kTwoBamboo),
  };
  const Meld meld_c = {
      .type = SetType::kConcealedKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("666z22m", &player);
  player.melds = {meld_a, meld_b, meld_c};
  player.meld_count = 3;
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeKans(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}
}  // namespace mahjong::yaku
