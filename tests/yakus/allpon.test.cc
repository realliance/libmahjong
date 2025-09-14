#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <string>

#include "analysis/analysis.h"
#include "analysis/handnode.h"
#include "scoring/yakus/allpons.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isAllPons, 2Han) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m222p888s666z44m", &hand);

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, WithKans) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m222p666z44m", &hand);
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kKan,
      .start = Piece(kFivePin),
  };
  hand.open = true;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, ConcealedKan) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m222p666z44m", &hand);
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kConcealedKan,
      .start = Piece(kFivePin),
  };
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, hand, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, BadHand) {
  auto game_state = GameState();
  Hand& hand = game_state.hands[0];
  HandFromNotation("111m222p666z789m44m", &hand);
  hand.open = false;

  auto root = breakdownHand(hand.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, hand, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
