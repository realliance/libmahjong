#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/threeconcealedpons.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {
TEST(isThreeConcealedPons, 2Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m111p111s666z44m", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealed) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m111p111s44m", &hand);
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kChi,
      .start = kTwoBamboo,
  };
  hand.open = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, CanHaveAdditionalOpenPon) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m111p111s44m", &hand);
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = kTwoBamboo,
  };
  hand.open = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealedNegative) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m111p234s44m", &hand);
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = kOneBamboo,
  };
  hand.open = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeConcealedPons, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m111p234567s44m", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
