#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/statefunction.h"
#include "types/walls.h"
#include "types/winds.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> RoundStart(std::unique_ptr<GameState> state) {
  Walls::New(*state);
  for (Hand& player : state->players) {
    auto hand = Walls::TakeHand(*state);
    state->controllers.at(player.id)->RoundStart(
        hand, static_cast<Wind>((player.id + 3 * (state->roundNum % 4)) % 4),
        (state->roundNum > 3) ? kSouth : kEast);
    player.live_count = hand.size();
    std::ranges::move(hand, player.live.begin());
  }

  AlertPlayers(*state,
               Event{
                   .type = Event::kDora,  // type
                   .player = -1,          // player
                   .piece = static_cast<int16_t>(
                       Walls::GetDoras(*state)[0].toUint8_t()),  // piece
                   .decision = false,                            // decision
               });

  state->nextState = StateFunctionType::kDraw;
  return state;
}
}  // namespace

REGISTER_ROUTE(RoundStart, StateFunctionType::kRoundStart);
}  // namespace mahjong
