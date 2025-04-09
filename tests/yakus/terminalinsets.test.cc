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

TEST(isTerminalsInAllSets, 2Han) {
  Meld meld = {
      .type = Meld::kChi,
      .start = Piece(kSevenPin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m111p11s"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTerminalsInAllSets(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, 3Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m111p789p11s"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTerminalsInAllSets(game_state, 0, branch) == 3) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, BadHand) {
  Meld meld = {
      .type = Meld::kChi,
      .start = Piece(kSevenPin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m222p11s"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTerminalsInAllSets(game_state, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
