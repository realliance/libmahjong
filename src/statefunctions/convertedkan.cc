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

auto mahjong::ConvertedKan(GameState& state) -> GameState& {
  AlertPlayers(state, Event{
                          .type = Event::kConvertedKan,   // type
                          .player = state.currentPlayer,  // player
                          .piece = static_cast<int16_t>(
                              state.pendingPiece.toUint8_t()),  // piece
                          .decision = false,                    // decision
                      });
  if (RemovePieces(state, state.currentPlayer, state.pendingPiece,
                   /*count=*/1) != 1) {
    std::cerr << "Not Enough pieces to remove in ConvertedKan" << '\n';
    state.nextState = Error;
    return state;
  }
  state.concealedKan = false;
  for (auto& meld : state.hands.at(state.currentPlayer).melds) {
    if (meld.type == Meld::kPon && meld.start == state.pendingPiece) {
      meld.type = Meld::kKan;
      state.nextState = KanDiscard;
      return state;
    }
  }
  std::cerr << "Could Not find matching pon" << '\n';
  state.nextState = Error;
  return state;
}
