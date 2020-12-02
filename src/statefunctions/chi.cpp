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

using Mahjong::GameState, Mahjong::Piece;

auto GetChiStart(const GameState& state, int player) -> Piece {
  if (CountPieces(state, player, state.pendingPiece - 2) > 0 && CountPieces(state, player, state.pendingPiece - 1) > 0) {
    return state.pendingPiece - 2;
  }
  if (CountPieces(state, player, state.pendingPiece - 1) > 0 && CountPieces(state, player, state.pendingPiece + 1) > 0) {
    return state.pendingPiece - 1;
  }
  if (CountPieces(state, player, state.pendingPiece + 1) > 0 && CountPieces(state, player, state.pendingPiece + 2) > 0) {
    return state.pendingPiece;
  }
  return Piece::ERROR;
}

auto Mahjong::Chi(GameState& state) -> GameState& {
  // only gives a single one of the chis
  // ui oof
  Piece chiStart = GetChiStart(state, state.lastCaller);
  if (chiStart == Piece::ERROR) {
    std::cerr << "Failed to get start of Chi" << std::endl;
    state.nextState = Error;
    return state;
  }

  if (state.hands.at(state.currentPlayer).riichi &&
      state.hands.at(state.currentPlayer).discards.size() == state.hands.at(state.currentPlayer).riichiPieceDiscard) {
    state.hands.at(state.currentPlayer).riichiPieceDiscard++;
  }

  state.hands.at(state.lastCaller).open = true;
  state.currentPlayer = state.lastCaller;

  AlertPlayers(state, Event{
                        Event::Chi,                                  // type
                        state.lastCaller,                            // player
                        static_cast<int16_t>(chiStart.toUint8_t()),  // piece
                        false,                                       // decision
                      });

  state.hands.at(state.lastCaller).live.push_back(state.pendingPiece);
  state.hands.at(state.lastCaller).sort();
  state.lastCall = state.turnNum;
  state.concealedKan = false;
  state.turnNum++;

  if (RemovePieces(state, state.lastCaller, chiStart, /*count=*/1) != 1 || RemovePieces(state, state.lastCaller, chiStart + 1, /*count=*/1) != 1 || RemovePieces(state, state.lastCaller, chiStart + 2, /*count=*/1) != 1) {
    std::cerr << "Not Enough Pieces to remove in Chi" << std::endl;
    state.nextState = Error;
    return state;
  }
  state.hands.at(state.lastCaller).melds.push_back({Meld::Chi, chiStart});

  state.pendingPiece = AskForDiscard(state);

  state.nextState = Discard;
  return state;
}
