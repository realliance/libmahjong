#include "scoring/yakus/fullflush.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isFullFlush, 5Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  hand.live = HandFromNotation("111m222m345m99m");
  hand.open = true;
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };
  ;

  auto root = breakdownHand(hand.live);

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
  Hand& hand = game_state.hands[0];
  hand.live = HandFromNotation("111m222m345m666m99m");
  hand.open = false;

  auto root = breakdownHand(hand.live);

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
  Hand& hand = game_state.hands[0];
  hand.live = HandFromNotation("111m222m111z666m99m");
  hand.open = false;

  auto root = breakdownHand(hand.live);

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
  Hand& hand = game_state.hands[0];
  hand.live = HandFromNotation("111m222m111p666m99m");
  hand.open = false;

  auto root = breakdownHand(hand.live);

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
  Hand& hand = game_state.hands[0];
  hand.live = HandFromNotation("111z222z333z444z55z");
  hand.open = false;

  auto root = breakdownHand(hand.live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
