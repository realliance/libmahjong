#include "scoring/yakus/halfflush.h"

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

TEST(isHalfFlush, 2Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("11m234m111z222z", &player);
  player.open = true;
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, 3Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("11m234m777m111z222z", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, BadPlayer) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("11m234m111p222z", &player);
  player.open = true;
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isHalfFlush, FullFlushIncompatible) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("11m234m888m777m", &player);
  player.open = true;
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isHalfFlush(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
