#include "scoring/yakus/littlethreedragons.h"

#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

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

TEST(isLittleThreeDragons, 2Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("555z666z111m222p77z", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isLittleThreeDragons(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isLittleThreeDragons, WhenOpen) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("666z111m222p77z", &hand);
  hand.open = true;
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kWhiteDragon),
  };

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isLittleThreeDragons(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isLittleThreeDragons, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("666z111m222p77s", &hand);
  hand.open = true;
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kWhiteDragon),
  };

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isLittleThreeDragons(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
