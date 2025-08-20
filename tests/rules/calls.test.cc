#include <gtest/gtest.h>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "controllers/playercontroller.h"
#include "statefunctions/statecontroller.h"
#include "types/event.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/statefunction.h"
#include "utils/gamestate_utils.h"
#include "utils/handformer.h"
#include "utils/playercontrollerfake.h"

namespace mahjong {

TEST(Calls, AcceptPon) {
  // Player 0: Has a 9 bamboo, and discards it
  // Player 1: Has a pair of 9 bamboo, will pon when player 0 discards

  std::vector<std::unique_ptr<PlayerController>> controllers;

  // Player 0
  controllers.push_back(
      std::make_unique<PlayerControllerFake>([](const Event& e) -> Event {
        // Discard 9 Bamboo
        if (e.type == Event::kDiscard) {
          Event decision = e;
          decision.piece = static_cast<uint16_t>(Piece::kNineBamboo);
          return decision;
        }

        return PlayerControllerFake::DefaultDecisionCallback(e);
      }));

  // Player 1
  controllers.push_back(
      std::make_unique<PlayerControllerFake>([](const Event& e) -> Event {
        // Accept all Pons
        if (e.type == Event::kPon) {
          return e;
        }

        return PlayerControllerFake::DefaultDecisionCallback(e);
      }));

  // Player 2 and 3 are set to default decline all
  auto state = InitializeTestRound(12345, std::move(controllers));

  // Override with hopeless hands
  state->hands[0].live = HandFromNotation("1m2p3s4m5p6s7m8p9s1m2p3s4m");
  // Has the pair of nine bamboo
  state->hands[1].live = HandFromNotation("1m3m5m7m9m99s2p4p6p1z3z5z7z");

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
  EXPECT_EQ(state->hands[1].melds.size(), 1);
  EXPECT_EQ(state->hands[1].melds[0].type, Meld::kPon);
  EXPECT_EQ(state->hands[1].melds[0].start, Piece(Piece::kNineBamboo));

  // Player 1's hand should be reduced by 2 tiles (used for pon)
  EXPECT_EQ(state->hands[1].live.size(), 11);
}

}  // namespace mahjong
