#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>
#include <memory>

#include "controllers/playercontroller.h"
#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/player.h"
#include "types/walls.h"
#include "types/winds.h"

namespace mahjong {

GameState&& RoundStart(GameState&& state) {
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
  return std::move(state);
}
}  // namespace mahjong
