#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/threeconcealedpons.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {
TEST(isThreeConcealedPons, 2Han) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("111m111p111s666z44m");
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealed) {
  auto meld = Meld();
  meld.type = SetType::kChi;
  meld.start = Piece(kTwoBamboo);

  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("111m111p111s44m");
  game_state.hands[0].melds.push_back(meld);
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, CanHaveAdditionalOpenPon) {
  auto meld = Meld();
  meld.type = SetType::kPon;
  meld.start = Piece(kTwoBamboo);

  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("111m111p111s44m");
  game_state.hands[0].melds.push_back(meld);
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealedNegative) {
  auto meld = Meld();
  meld.type = SetType::kPon;
  meld.start = Piece(kOneBamboo);

  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("111m111p234s44m");
  game_state.hands[0].melds.push_back(meld);
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeConcealedPons, BadHand) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("111m111p234567s44m");
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isThreeConcealedPons(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
