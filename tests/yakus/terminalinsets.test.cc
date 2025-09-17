#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/terminalsinallsets.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isTerminalsInAllSets, 2Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123m789m111p11s", &hand);
  hand.open = true;
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kChi,
      .start = Piece(kSevenPin),
  };

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTerminalsInAllSets(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, 3Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123m789m111p789p11s", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTerminalsInAllSets(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("123m789m222p11s", &hand);
  hand.open = true;
  hand.open = true;
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kChi,
      .start = Piece(kSevenPin),
  };

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isTerminalsInAllSets(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
