#include <gtest/gtest.h>

#include <array>
#include <string>

#include "scoring/yakus/fullyconcealedhand.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/settings.h"
#include "utils/handformer.h"

namespace mahjong::yaku {
TEST(isFullyConcealedHand, 1Han) {
  auto game_state = GameState();
  Player& player = game_state.players[0];
  HandFromNotation("555m555p555s111z44m", &player);
  player.open = false;
  game_state.currentPlayer = 0;

  EXPECT_TRUE(isFullyConcealedHand(game_state, player));
}

TEST(isFullyConcealedHand, MustTsumo) {
  auto game_state = GameState();
  Player& player = game_state.players[0];
  HandFromNotation("555m555p555s111z44m", &player);
  player.open = false;
  game_state.currentPlayer = 1;

  EXPECT_FALSE(isFullyConcealedHand(game_state, player));
}

TEST(isFullyConcealedHand, MustBeClosedPlayer) {
  auto game_state = GameState();
  Player& player = game_state.players[0];
  HandFromNotation("555m555p555s111z44m", &player);
  player.open = true;
  game_state.currentPlayer = 0;

  EXPECT_FALSE(isFullyConcealedHand(game_state, player));
}

TEST(isFullyConcealedHand, MustHavePiecesRemainingInWall) {
  auto game_state = GameState();
  Player& player = game_state.players[0];
  HandFromNotation("555m555p555s111z44m", &player);
  player.open = false;
  game_state.currentPlayer = 0;

  // Empty the Wall
  game_state.livingWallIndex = kLivingWallCount;

  EXPECT_FALSE(isFullyConcealedHand(game_state, player));
}
}  // namespace mahjong::yaku
