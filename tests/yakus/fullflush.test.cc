#include "scoring/yakus/fullflush.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isFullFlush, 5Han) {
  const Meld meld = {
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222m345m99m"));
  game_state.hands[0].open = true;
  game_state.hands[0].melds = {meld};

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFullFlush, 6Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222m345m666m99m"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFullFlush, BadHandHonors) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222m111z666m99m"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isFullFlush, BadHandSuit) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222m111p666m99m"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isFullFlush, BadHandFullFlushHonors) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111z222z333z444z55z"));
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
