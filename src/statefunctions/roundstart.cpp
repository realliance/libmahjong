#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"
#include "stateutilities.h"
#include "walls.h"
#include "winds.h"

auto Mahjong::RoundStart(GameState& state) -> GameState& {
  if (state.overrideWall.empty()) {
    state.walls = Walls(state.g);
  } else {
    state.walls = Walls(state.overrideWall);
  }
  for (size_t i = 0; i < 4; i++) {
    auto hand = state.walls.TakeHand();
    state.players.at(i).controller->RoundStart(
      hand,
      static_cast<Wind>((i + 3 * (state.roundNum % 4)) % 4),
      (state.roundNum > 3) ? South : East);
    state.hands.at(i) = Hand(hand);
  }

  AlertPlayers(state, Event{
                        Event::Dora,                                                  // type
                        -1,                                                           // player
                        static_cast<int16_t>(state.walls.GetDoras()[0].toUint8_t()),  // piece
                        false,                                                        // decision
                      });

  state.nextState = Draw;
  return state;
}
