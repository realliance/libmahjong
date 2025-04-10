#include <gtest/gtest.h>
#include <array>
#include <string>

#include "analysis/hands.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(isRiichi, 1Han) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  game_state.hands[0].riichi = true;

  game_state.hands[0].riichiRound = 7;
  game_state.turnNum = 15;
  game_state.lastCall = 2;

  EXPECT_EQ(isRiichi(game_state, 0), 1);
}

TEST(isRiichi, Ippatsu) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  game_state.hands[0].riichi = true;

  game_state.hands[0].riichiRound = 14;
  game_state.turnNum = 15;
  game_state.lastCall = 2;

  EXPECT_EQ(isRiichi(game_state, 0), 2);
}

TEST(isRiichi, DoubleRiichi) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  game_state.hands[0].riichi = true;

  game_state.hands[0].riichiRound = 1;
  game_state.turnNum = 2;
  game_state.lastCall = -1;

  EXPECT_EQ(isRiichi(game_state, 0), 3);
}

TEST(isRiichi, NoRiichi) {
  auto game_state = GameState();
  game_state.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  game_state.hands[0].riichi = false;

  game_state.turnNum = 2;
  game_state.lastCall = -1;

  EXPECT_EQ(isRiichi(game_state, 0), 0);
}
}  // namespace mahjong
