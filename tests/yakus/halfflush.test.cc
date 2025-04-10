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

TEST(isHalfFlush, 2Han) {
  Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kSixCharacter),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m234m111z222z"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isHalfFlush(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, 3Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m234m777m111z222z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isHalfFlush(game_state, 0, branch) == 3) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, BadHand) {
  Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kSixCharacter),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m234m111p222z"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isHalfFlush(game_state, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isHalfFlush, FullFlushIncompatible) {
  Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kSixCharacter),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m234m888m777m"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isHalfFlush(game_state, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
