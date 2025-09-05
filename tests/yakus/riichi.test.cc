#include "scoring/yakus/riichi.h"

#include <gtest/gtest.h>

#include <array>
#include <string>

#include "scoring/yakus/doubleriichi.h"
#include "scoring/yakus/ippatsu.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "utils/handformer.h"

namespace mahjong::yaku {

TEST(isRiichi, Riichi) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("555m555p555s111z44m", &player);
  player.riichi = true;

  player.riichiRound = 7;
  game_state.turnNum = 15;
  game_state.lastCall = 2;

  EXPECT_TRUE(isRiichi(game_state, player));
}

TEST(isRiichi, Ippatsu) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("555m555p555s111z44m", &player);
  player.riichi = true;

  player.riichiRound = 14;
  game_state.turnNum = 15;
  game_state.lastCall = 2;

  EXPECT_TRUE(isIppatsu(game_state, player));
}

TEST(isRiichi, DoubleRiichi) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("555m555p555s111z44m", &player);
  player.riichi = true;

  player.riichiRound = 1;
  game_state.turnNum = 2;
  game_state.lastCall = -1;

  EXPECT_TRUE(isDoubleRiichi(game_state, player));
}

TEST(isRiichi, NoRiichi) {
  auto game_state = GameState();
  Hand& player = game_state.players[0];
  HandFromNotation("555m555p555s111z44m", &player);
  player.riichi = false;

  game_state.turnNum = 2;
  game_state.lastCall = -1;

  EXPECT_FALSE(isRiichi(game_state, player));
}
}  // namespace mahjong::yaku
