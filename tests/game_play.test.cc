#include <gtest/gtest.h>
#include <array>
#include <vector>

#include "analysis/hands.h"
#include "statefunctions/decisionfunction.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {

// TEST(GamePlay, Discard) {
//   std::unique_ptr<GameState> state;
//   state->overrideWall = {kSixBamboo};
//   PlayerControllerFake bot_ptr;
//   std::unique_ptr<PlayerControllerFake> bot;

//   for (int i = 0; i < 4; i++) {
//     state->players[i].controller = std::move(bot);
//   }
//   state = RoundStart(std::move(state));
//   state = Draw(std::move(state));
//   const Event e = {
//       .type = Event::kDiscard,
//       .player = 0,
//       .piece = Piece(kSixBamboo).toUint8_t(),
//       .decision = true,
//   };
//   bot_ptr.AddEvents({e});
//   ASSERT_NO_THROW(state = PlayerHand(std::move(state)));
//   EXPECT_EQ(e, bot_ptr.GetEvents()[0]);
//   EXPECT_EQ(state->hands[0].discards[0], kSixBamboo);
// }

TEST(GamePlay, Furiten) {
  GameState state;
  state.pendingPiece = kRedDragon;
  state.hands[0].live = {kRedDragon, kRedDragon, kRedDragon};
  ASSERT_TRUE(isComplete(state, 0));
  state.hands[0].live = {kRedDragon, kRedDragon};
  EXPECT_FALSE(CanRon(state, 0));
}

}  // namespace mahjong
