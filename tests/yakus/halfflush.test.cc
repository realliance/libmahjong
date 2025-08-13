#include "scoring/yakus/halfflush.h"
#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "utils/handformer.h"

namespace mahjong::yakus {

TEST(isHalfFlush, 2Han) {
  const Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kSixCharacter),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m234m111z222z"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, 0, branch)) {
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
    if (isHalfFlush(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, BadHand) {
  const Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kSixCharacter),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m234m111p222z"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isHalfFlush, FullFlushIncompatible) {
  const Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kSixCharacter),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("11m234m888m777m"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yakus
