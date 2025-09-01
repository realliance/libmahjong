#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

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
  game_state.hands[0] = Hand(HandFromNotation("666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeKans(game_state, 0, branch)) {
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
  game_state.hands[0] = Hand(HandFromNotation("22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c, meld_d};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeKans(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, BadHand) {
  const Meld meld_a = {
      .type = SetType::kConcealedKan,
      .start = Piece(kOneBamboo),
  };
  const Meld meld_b = {
      .type = SetType::kKan,
      .start = Piece(kTwoBamboo),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("777p666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeKans(game_state, 0, branch)) {
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
  game_state.hands[0] = Hand(HandFromNotation("666z22m"));
  game_state.hands[0].melds = {meld_a, meld_b, meld_c};
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeKans(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}
}  // namespace mahjong::yaku
