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

auto Mahjong::Kan(GameState& state) -> GameState& {
  AlertPlayers(state, Event{
                        Event::Kan,                                            // type
                        state.lastCaller,                                      // player
                        static_cast<int16_t>(state.pendingPiece.toUint8_t()),  // piece
                        false,                                                 // decision
                      });

  if (state.hands.at(state.currentPlayer).riichi &&
      state.hands.at(state.currentPlayer).discards.size() == state.hands.at(state.currentPlayer).riichiPieceDiscard) {
    state.hands.at(state.currentPlayer).riichiPieceDiscard++;
  }

  state.hands.at(state.lastCaller).open = true;
  state.currentPlayer = state.lastCaller;
  state.hands.at(state.lastCaller).live.push_back(state.pendingPiece);
  state.hands.at(state.lastCaller).sort();
  state.lastCall = state.turnNum;
  state.concealedKan = false;
  state.turnNum++;

  if (RemovePieces(state, state.lastCaller, state.pendingPiece, /*count=*/4) != 4) {
    std::cerr << "Not Enough Pieces to remove in kan" << std::endl;
    state.nextState = Error;
    return state;
  }
  state.hands.at(state.lastCaller).melds.push_back({Meld::Kan, state.pendingPiece});

  state.nextState = KanDiscard;
  return state;
}
