#include <gtest/gtest.h>
#include <array>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "analysis/hands.h"
#include "statefunctions/decisionfunction.h"
#include "statefunctions/statefunctions.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/player.h"
#include "utils/playercontrollerfake.h"
#include "controllers/playercontroller.h"

namespace mahjong {

TEST(GamePlay, Discard) {
  GameState state;
  state.overrideWall = {kSixBamboo};
  PlayerControllerFake* bot_ptr;
  std::unique_ptr<PlayerControllerFake> bot;

  for (int i = 0; i < 4; i++) {
    state.players[i].controller = std::move(bot);
  }
  state = RoundStart(std::move(state));
  state = Draw(std::move(state));
  Event e = {
      .type = Event::kDiscard,
      .player = 0,
      .piece = Piece(kSixBamboo).toUint8_t(),
      .decision = true,
  };
  bot_ptr->AddEvents({e});
  ASSERT_NO_THROW(state = PlayerHand(std::move(state)));
  EXPECT_EQ(e, bot_ptr->GetEvents()[0]);
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
