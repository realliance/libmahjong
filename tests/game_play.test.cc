#include <gtest/gtest.h>

#include <array>

#include "scoring/scoring.h"
#include "statefunctions/decisionfunction.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(GamePlay, Furiten) {
  auto state = GameState();
  Player& player = state.players[0];
  HandFromNotation("123m456p234678s44m", &player);
  player.open = false;
  EXPECT_TRUE(isComplete(state, player));
  state.pendingPiece = Piece(kFourPin);
  // Place Four Pin in Discard Pile
  player.discards = {kFourPin};
  EXPECT_FALSE(CanRon(state, player));
}

}  // namespace mahjong
