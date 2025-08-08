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

TEST(isAllTerminalsAndHonors, 2Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("222z111p111m999s66z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllTerminalsAndHonors(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllTerminalsAndHonors, BadHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("222z111p111m888s66z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllTerminalsAndHonors(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isAllTerminalsAndHonors, CanBeOpen) {
  const Meld meld = {
      .type = Meld::kPon,
      .start = Piece(kNinePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("222z111m999s66z"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (mahjong::isAllTerminalsAndHonors(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}
}  // namespace mahjong
