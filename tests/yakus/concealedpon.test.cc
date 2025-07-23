#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"
#include "analysis/hands.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "utils/handformer.h"

namespace mahjong {
TEST(isThreeConcealedPons, DISABLED_2Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m111p111s666z44m"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeConcealedPons(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, DISABLED_PonsMustBeConcealed) {
  auto meld = Meld();
  meld.type = Meld::kChi;
  meld.start = Piece(kTwoBamboo);

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m111p111s44m"));
  game_state.hands[0].melds.push_back(meld);
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeConcealedPons(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, DISABLED_CanHaveAdditionalOpenPon) {
  auto meld = Meld();
  meld.type = Meld::kPon;
  meld.start = Piece(kTwoBamboo);

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m111p111s44m"));
  game_state.hands[0].melds.push_back(meld);
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeConcealedPons(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealedNegative) {
  auto meld = Meld();
  meld.type = Meld::kPon;
  meld.start = Piece(kOneBamboo);

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m111p234s44m"));
  game_state.hands[0].melds.push_back(meld);
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeConcealedPons(game_state, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeConcealedPons, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m111p234567s44m"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isThreeConcealedPons(game_state, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
