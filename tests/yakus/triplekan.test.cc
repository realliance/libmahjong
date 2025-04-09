#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis.h"
#include "gamestate.h"
#include "hand.h"
#include "handformer.h"
#include "handnode.h"
#include "hands.h"
#include "meld.h"
#include "pieces.h"
#include "piecetype.h"

namespace mahjong {

TEST(isThreeKans, 2Han) {
  Meld meld_a = {
      .type = Meld::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  Meld meld_b = {
      .type = Meld::kKan,
      .start = Piece(kTwoBamboo),
  };
  Meld meld_c = {
      .type = Meld::kKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeKans(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, WithOtherMelds) {
  Meld meld_a = {
      .type = Meld::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  Meld meld_b = {
      .type = Meld::kKan,
      .start = Piece(kTwoBamboo),
  };
  Meld meld_c = {
      .type = Meld::kKan,
      .start = Piece(kFivePin),
  };
  Meld meld_d = {
      .type = Meld::kChi,
      .start = Piece(kTwoBamboo),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c, meld_d};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeKans(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, BadHand) {
  Meld meld_a = {
      .type = Meld::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  Meld meld_b = {
      .type = Meld::kKan,
      .start = Piece(kTwoBamboo),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("777p666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeKans(game_state, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeKans, AllConcealed) {
  Meld meld_a = {
      .type = Meld::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  Meld meld_b = {
      .type = Meld::kConcealedKan,
      .start = Piece(kTwoBamboo),
  };
  Meld meld_c = {
      .type = Meld::kConcealedKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c};
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeKans(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}
}  // namespace mahjong
