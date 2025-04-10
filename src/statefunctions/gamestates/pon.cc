#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/piecetype.h"

namespace mahjong {

GameState& Pon(GameState& state) {
  state.hands.at(state.lastCaller).open = true;

  AlertPlayers(state, Event{
                          .type = Event::kPon,         // type
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

  state.currentPlayer = state.lastCaller;
  state.hands.at(state.lastCaller).live.push_back(state.pendingPiece);
  state.hands.at(state.lastCaller).sort();
  state.lastCall = state.turnNum;
  state.concealedKan = false;
  state.turnNum++;

  if (RemovePieces(state, state.lastCaller, state.pendingPiece, /*count=*/3) !=
      3) {
    std::cerr << "Not enough pieces to remove in Pon" << '\n';
    state.nextState = Error;
    return state;
  }
  state.hands.at(state.lastCaller)
      .melds.push_back({Meld::kPon, state.pendingPiece});

  state.pendingPiece = AskForDiscard(state);

  state.nextState = Discard;
  return state;
}

}  // namespace mahjong
