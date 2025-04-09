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

TEST(isAllPons, 2Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222p888s666z44m"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllPons(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, WithKans) {
  Meld meld = {
      .type = Meld::kKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  game_state.hands[0].melds = {meld};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllPons(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, ConcealedKan) {
  Meld meld = {
      .type = Meld::kConcealedKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  game_state.hands[0].melds = {meld};
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllPons(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222p666z789m44m"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllPons(game_state, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
