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

auto Mahjong::Pon(GameState& state) -> GameState& {
  state.hands.at(state.lastCaller).open = true;

  AlertPlayers(state, Event{
                        Event::Pon,                                            // type
                        state.lastCaller,                                      // player
                        static_cast<int16_t>(state.pendingPiece.toUint8_t()),  // piece
                        false,                                                 // decision
                      });

  if (state.hands.at(state.currentPlayer).riichi &&
      state.hands.at(state.currentPlayer).discards.size() == state.hands.at(state.currentPlayer).riichiPieceDiscard) {
    state.hands.at(state.currentPlayer).riichiPieceDiscard++;
  }

  state.currentPlayer = state.lastCaller;
  state.hands.at(state.lastCaller).live.push_back(state.pendingPiece);
  state.hands.at(state.lastCaller).sort();
  state.lastCall = state.turnNum;
  state.concealedKan = false;
  state.turnNum++;

  if (RemovePieces(state, state.lastCaller, state.pendingPiece, /*count=*/3) != 3) {
    std::cerr << "Not enough pieces to remove in Pon" << std::endl;
    state.nextState = Error;
    return state;
  }
  state.hands.at(state.lastCaller).melds.push_back({Meld::Pon, state.pendingPiece});

  state.pendingPiece = AskForDiscard(state);

  state.nextState = Discard;
  return state;
}
