#include "scoring/yakus/riichi.h"

#include <gtest/gtest.h>

#include <array>
#include <string>

#include "scoring/yakus/doubleriichi.h"
#include "scoring/yakus/ippatsu.h"
#include "types/gamestate.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isRiichi, Riichi) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("555m555p555s111z44m");
  game_state.hands[0].riichi = true;

  game_state.hands[0].riichiRound = 7;
  game_state.turnNum = 15;
  game_state.lastCall = 2;

  EXPECT_TRUE(isRiichi(game_state, 0));
}

TEST(isRiichi, Ippatsu) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("555m555p555s111z44m");
  game_state.hands[0].riichi = true;

  game_state.hands[0].riichiRound = 14;
  game_state.turnNum = 15;
  game_state.lastCall = 2;

  EXPECT_TRUE(isIppatsu(game_state, 0));
}

TEST(isRiichi, DoubleRiichi) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("555m555p555s111z44m");
  game_state.hands[0].riichi = true;

  game_state.hands[0].riichiRound = 1;
  game_state.turnNum = 2;
  game_state.lastCall = -1;

  EXPECT_TRUE(isDoubleRiichi(game_state, 0));
}

TEST(isRiichi, NoRiichi) {
  auto game_state = GameState();
  game_state.hands[0].live = HandFromNotation("555m555p555s111z44m");
  game_state.hands[0].riichi = false;

  game_state.turnNum = 2;
  game_state.lastCall = -1;

  EXPECT_FALSE(isRiichi(game_state, 0));
}
}  // namespace mahjong::yaku
