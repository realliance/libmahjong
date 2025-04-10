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

namespace mahjong {

GameState& Kan(GameState& state) {
  AlertPlayers(state, Event{
                          .type = Event::kKan,         // type
                          .player = state.lastCaller,  // player
                          .piece = static_cast<int16_t>(
                              state.pendingPiece.toUint8_t()),  // piece
                          .decision = false,                    // decision
                      });

  if (state.hands.at(state.currentPlayer).riichi &&
      state.hands.at(state.currentPlayer).discards.size() ==
          state.hands.at(state.currentPlayer).riichiPieceDiscard) {
    state.hands.at(state.currentPlayer).riichiPieceDiscard++;
  }

  state.hands.at(state.lastCaller).open = true;
  state.currentPlayer = state.lastCaller;
  state.hands.at(state.lastCaller).live.push_back(state.pendingPiece);
  state.hands.at(state.lastCaller).sort();
  state.lastCall = state.turnNum;
  state.concealedKan = false;
  state.turnNum++;

  if (RemovePieces(state, state.lastCaller, state.pendingPiece, /*count=*/4) !=
      4) {
    std::cerr << "Not Enough Pieces to remove in kan" << '\n';
    state.nextState = Error;
    return state;
  }
  state.hands.at(state.lastCaller)
      .melds.push_back({Meld::kKan, state.pendingPiece});

  state.nextState = KanDiscard;
  return state;
}

}  // namespace mahjong
