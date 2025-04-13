#include <gtest/gtest.h>
#include <array>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/player.h"
#include "types/walls.h"
#include "types/winds.h"
#include "utils/playercontrollerfake.h"
#include "controllers/playercontroller.h"

namespace mahjong {

TEST(Setup, SeatWind) {
  EXPECT_EQ(GetSeat(0, 0), Wind::kEast);
  EXPECT_EQ(GetSeat(0, 1), Wind::kSouth);
  EXPECT_EQ(GetSeat(0, 2), Wind::kWest);
  EXPECT_EQ(GetSeat(0, 3), Wind::kNorth);
}

TEST(Setup, PrevalentWind) {
  // EXPECT_EQ(GetPrevalentWind(0), Wind::kEast);
  // EXPECT_EQ(GetPrevalentWind(4), Wind::kSouth);
  EXPECT_EQ(GetSeat(4, 0), Wind::kEast);
}

TEST(Setup, DoraIndicator) {
  GameState state;
  PlayerControllerFake bot;
  for (int i = 0; i < 4; i++) {
    state.players[i].controller = std::make_unique<PlayerControllerFake>();
    ;
  }
  ASSERT_NO_THROW(state = RoundStart(std::move(state)));
  EXPECT_EQ(state.walls.GetDoras().size(), 1);
}

TEST(Setup, Dealing) {
  GameState state;
  for (int i = 0; i < 4; i++) {
    state.players[i].controller = std::make_unique<PlayerControllerFake>();
  }
  ASSERT_NO_THROW(state = RoundStart(std::move(state)));
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(state.hands[i].live.size(), 13);
  }
  state = Draw(std::move(state));
  EXPECT_EQ(state.currentPlayer, 0);
}

}  // namespace mahjong
