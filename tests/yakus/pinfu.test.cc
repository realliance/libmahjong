#include "scoring/yakus/pinfu.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isPinfu, 1Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123m456p234678s44m", &hand);
  hand.open = false;
  game_state.pendingPiece = Piece(kFourPin);

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPinfu(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPinfu, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123m555p234678s44m", &hand);
  hand.open = false;
  game_state.pendingPiece = Piece(kTwoBamboo);

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPinfu(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPinfu, CantBeOpen) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123m456p234678s44m", &hand);
  hand.open = true;
  game_state.pendingPiece = Piece(kFourPin);

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPinfu(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPinfu, NeedTwoWait) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123m456p234678s44m", &hand);
  hand.open = false;
  game_state.pendingPiece = Piece(kFivePin);

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isPinfu(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
