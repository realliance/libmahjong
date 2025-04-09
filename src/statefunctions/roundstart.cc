#include <array>
#include <cstddef>
#include <cstdint>
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

auto mahjong::RoundStart(GameState& state) -> GameState& {
  if (state.overrideWall.empty()) {
    state.walls = Walls(state.g);
  } else {
    state.walls = Walls(state.overrideWall);
  }
  for (size_t i = 0; i < 4; i++) {
    auto hand = state.walls.TakeHand();
    state.players.at(i).controller->RoundStart(
        hand, static_cast<Wind>((i + 3 * (state.roundNum % 4)) % 4),
        (state.roundNum > 3) ? kSouth : kEast);
    state.hands.at(i) = Hand(hand);
  }

  AlertPlayers(state, Event{
                          .type = Event::kDora,  // type
                          .player = -1,          // player
                          .piece = static_cast<int16_t>(
                              state.walls.GetDoras()[0].toUint8_t()),  // piece
                          .decision = false,  // decision
                      });

  state.nextState = Draw;
  return state;
}
