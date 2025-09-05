#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <memory>
#include <utility>

#include "controllers/playercontroller.h"
#include "statefunctions/statecontroller.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/statefunction.h"
#include "utils/gamestate_utils.h"
#include "utils/handformer.h"
#include "utils/playercontrollerfake.h"

namespace mahjong {

TEST(Calls, AcceptPon) {
  // Player 0: Has a 9 bamboo, and discards it
  // Player 1: Has a pair of 9 bamboo, will pon when player 0 discards

  std::array<std::unique_ptr<PlayerController>, 4> controllers;

  // Player 0
  controllers[0] =
      std::make_unique<PlayerControllerFake>([](const Event& e) -> Event {
        // Discard 9 Bamboo
        if (e.type == Event::kDiscard) {
          Event decision = e;
          decision.piece = static_cast<uint16_t>(Piece::kNineBamboo);
          return decision;
        }

        return PlayerControllerFake::DefaultDecisionCallback(e);
      });

  // Player 1
  controllers[1] =
      std::make_unique<PlayerControllerFake>([](const Event& e) -> Event {
        // Accept all Pons
        if (e.type == Event::kPon) {
          return e;
        }

        return PlayerControllerFake::DefaultDecisionCallback(e);
      });

  // Player 2 and 3 are set to default decline all
  auto state = InitializeTestRound(12345, std::move(controllers));

  // Override with hopeless hands
  Hand& player0 = state->players[0];
  // 14 Pieces because has drawn in InitializeTestRound.
  HandFromNotation("11447m2258p33699s", &player0);
  // Has the pair of nine bamboo 13 Pieces, yet to draw.
  Hand& player1 = state->players[1];
  HandFromNotation("13579m99s246p135z", &player1);

  // Advance to pon event
  // Should take 4 iterations: draw, player hand, discard, and then the pon
  // Player 1 should pon
  state = AdvanceThroughState(std::move(state), StateFunctionType::kPon, 4);

  // Advance one more to then discard afterwards
  state = AdvanceGameState(std::move(state));

  // After successful pon:
  // - Player 1 should have an open meld
  // - Player 1 should be the current player
  // - State should be kDiscard (Player 1 needs to discard)
  EXPECT_EQ(state->currentPlayer, 1);
  EXPECT_EQ(state->currState, StateFunctionType::kDiscard);
  EXPECT_EQ(player1.meld_count, 1);
  EXPECT_EQ(player1.melds[0].type, SetType::kPon);
  EXPECT_EQ(player1.melds[0].start, Piece(Piece::kNineBamboo));

  // Player 1's live hand should be reduced by 3 tiles (pon + discard)
  EXPECT_EQ(player1.live_count, 10);
}

}  // namespace mahjong
