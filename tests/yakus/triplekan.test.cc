#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"
#include "scoring/yakus.h"

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(isThreeKans, 2Han) {
  const Meld meld_a = {
      .type = Meld::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = Meld::kKan,
      .start = Piece(kTwoBamboo),
  };
  const Meld meld_c = {
      .type = Meld::kKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeKans(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, WithOtherMelds) {
  const Meld meld_a = {
      .type = Meld::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = Meld::kKan,
      .start = Piece(kTwoBamboo),
  };
  const Meld meld_c = {
      .type = Meld::kKan,
      .start = Piece(kFivePin),
  };
  const Meld meld_d = {
      .type = Meld::kChi,
      .start = Piece(kTwoBamboo),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c, meld_d};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeKans(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, BadHand) {
  const Meld meld_a = {
      .type = Meld::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = Meld::kKan,
      .start = Piece(kTwoBamboo),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("777p666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeKans(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeKans, AllConcealed) {
  const Meld meld_a = {
      .type = Meld::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = Meld::kConcealedKan,
      .start = Piece(kTwoBamboo),
  };
  const Meld meld_c = {
      .type = Meld::kConcealedKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c};
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeKans(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}
}  // namespace mahjong
