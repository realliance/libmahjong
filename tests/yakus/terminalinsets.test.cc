#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"
#include "analysis/yakus.h"

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(isTerminalsInAllSets, 2Han) {
  const Meld meld = {
      .type = Meld::kChi,
      .start = Piece(kSevenPin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m111p11s"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTerminalsInAllSets(game_state, 0, branch)) {
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
    if (mahjong::isTerminalsInAllSets(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, BadHand) {
  const Meld meld = {
      .type = Meld::kChi,
      .start = Piece(kSevenPin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m789m222p11s"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isTerminalsInAllSets(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
