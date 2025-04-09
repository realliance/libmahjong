#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "meld.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"

auto mahjong::ConcealedKan(GameState& state) -> GameState& {
  AlertPlayers(state, Event{
                          .type = Event::kConcealedKan,   // type
                          .player = state.currentPlayer,  // player
                          .piece = static_cast<int16_t>(
                              state.pendingPiece.toUint8_t()),  // piece
                          .decision = false,                    // decision
                      });
  if (RemovePieces(state, state.currentPlayer, state.pendingPiece,
                   /*count=*/4) != 4) {
    std::cerr << "Not Enough pieces to remove in ConcealedKan" << '\n';
    state.nextState = Error;
    return state;
  }
  state.hands.at(state.currentPlayer)
      .melds.push_back({Meld::kConcealedKan, state.pendingPiece});
  state.concealedKan = true;
  state.nextState = KanDiscard;
  return state;
}
