#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>
#include <vector>

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
  game_state.hands[0] = Hand(HandFromNotation("111m222p888s666z44m"));

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, WithKans) {
  const Meld meld = {
      .type = SetType::kKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  game_state.hands[0].melds = {meld};
  game_state.hands[0].open = true;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, 0, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, ConcealedKan) {
  const Meld meld = {
      .type = SetType::kConcealedKan,
      .start = Piece(kFivePin),
  };

  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  game_state.hands[0].melds = {meld};
  game_state.hands[0].open = false;

  auto root = breakdownHand(game_state.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isAllPons(game_state, 0, branch)) {
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
    if (isAllPons(game_state, 0, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
