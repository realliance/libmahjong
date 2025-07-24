#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/hands.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(isPinfu, DISABLED_1Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456p234s678z44m"));
  game_state.hands[0].open = false;
  game_state.pendingPiece = Piece(kFourPin);

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isPinfu(game_state, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPinfu, DISABLED_BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m555p234s678z44m"));
  game_state.hands[0].open = false;
  game_state.pendingPiece = Piece(kTwoBamboo);

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isPinfu(game_state, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPinfu, DISABLED_CantBeOpen) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456p234s678z44m"));
  game_state.hands[0].open = true;
  game_state.pendingPiece = Piece(kFourPin);

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isPinfu(game_state, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPinfu, DISABLED_NeedTwoWait) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("123m456p234s678z44m"));
  game_state.hands[0].open = false;
  game_state.pendingPiece = Piece(kFivePin);

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isPinfu(game_state, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
