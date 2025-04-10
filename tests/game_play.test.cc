#include <gtest/gtest.h>
#include <array>
#include <string>
#include <vector>

#include "analysis/hands.h"
#include "statefunctions/decisionfunction.h"
#include "statefunctions/statefunctions.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/player.h"
#include "utils/testai.h"

namespace mahjong {

TEST(GamePlay, Discard) {
  GameState state;
  state.overrideWall = {kSixBamboo};
  TesterBot bot;
  for (int i = 0; i < 4; i++) {
    state.players[i].controller = &bot;
  }
  RoundStart(state);
  Draw(state);
  Event e = {
      .type = Event::kDiscard,
      .player = 0,
      .piece = Piece(kSixBamboo).toUint8_t(),
      .decision = true,
  };
  bot.AddEvents({e});
  ASSERT_NO_THROW(PlayerHand(state));
  EXPECT_EQ(e, bot.GetEvents()[0]);
  EXPECT_EQ(state.hands[0].discards[0], kSixBamboo);
}

TEST(GamePlay, Furiten) {
  GameState state;
  state.pendingPiece = kRedDragon;
  state.hands[0].live = {kRedDragon, kRedDragon, kRedDragon};
  ASSERT_TRUE(isComplete(state, 0));
  state.hands[0].live = {kRedDragon, kRedDragon};
  EXPECT_FALSE(CanRon(state, 0));
}

}  // namespace mahjong
