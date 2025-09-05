#include "scoring/yakus/fullflush.h"

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

TEST(isFullFlush, 5Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m222m345m99m", &player);
  player.open = true;
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = Piece(kSixCharacter),
  };

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFullFlush, 6Han) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m222m345m666m99m", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, player, branch)) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFullFlush, BadPlayerHonors) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m222m111z666m99m", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isFullFlush, BadPlayerSuit) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111m222m111p666m99m", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isFullFlush, BadPlayerFullFlushHonors) {
  auto game_state = GameState();
  Player& player =game_state.players[0];
  HandFromNotation("111z222z333z444z55z", &player);
  player.open = false;

  auto root = breakdownHand(player.live_range());

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (isFullFlush(game_state, player, branch)) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
}  // namespace mahjong::yaku
