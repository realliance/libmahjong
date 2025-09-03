#include <gtest/gtest.h>

#include <array>
#include <string>
#include <vector>

#include "scoring/yakus/fullyconcealedhand.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/walls.h"
#include "utils/handformer.h"

namespace mahjong::yaku {
TEST(isFullyConcealedHand, 1Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  game_state.hands[0].open = false;
  game_state.currentPlayer = 0;

  EXPECT_TRUE(isFullyConcealedHand(game_state, 0));
}

TEST(isFullyConcealedHand, MustTsumo) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  game_state.hands[0].open = false;
  game_state.currentPlayer = 1;

  EXPECT_FALSE(isFullyConcealedHand(game_state, 0));
}

TEST(isFullyConcealedHand, MustBeClosedHand) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  game_state.hands[0].open = true;
  game_state.currentPlayer = 0;

  EXPECT_FALSE(isFullyConcealedHand(game_state, 0));
}

TEST(isFullyConcealedHand, MustHavePiecesRemainingInWall) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  game_state.hands[0].open = false;
  game_state.currentPlayer = 0;

  // Empty the Wall
  game_state.walls.livingWalls.clear();

  EXPECT_FALSE(isFullyConcealedHand(game_state, 0));
}
}  // namespace mahjong::yaku
