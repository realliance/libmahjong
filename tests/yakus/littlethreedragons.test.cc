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

TEST(isLittleThreeDragons, 2Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555z666z111m222p77z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isLittleThreeDragons(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isLittleThreeDragons, WhenOpen) {
  const Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kWhiteDragon),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("666z111m222p77z"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isLittleThreeDragons(game_state, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isLittleThreeDragons, BadHand) {
  const Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kWhiteDragon),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("666z111m222p77s"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isLittleThreeDragons(game_state, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong
