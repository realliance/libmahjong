#include <gtest/gtest.h>

#include <array>

#include "scoring/scoring.h"
#include "statefunctions/decisionfunction.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "utils/handformer.h"

namespace mahjong {

TEST(GamePlay, Furiten) {
  auto state = GameState();
  Hand& hand = state.hands[0];
  HandFromNotation("123m456p234678s44m", &hand);
  hand.open = false;
  EXPECT_TRUE(isComplete(state, 0));
  state.pendingPiece = Piece(kFourPin);
  // Place Four Pin in Discard Pile
  hand.discards = {kFourPin};
  EXPECT_FALSE(CanRon(state, 0));
}

}  // namespace mahjong
