#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {
namespace {
Piece GetChiStart(const GameState& state, int player) {
  if (CountPieces(state, player, state.pendingPiece - 2) > 0 &&
      CountPieces(state, player, state.pendingPiece - 1) > 0) {
    return state.pendingPiece - 2;
  }
  if (CountPieces(state, player, state.pendingPiece - 1) > 0 &&
      CountPieces(state, player, state.pendingPiece + 1) > 0) {
    return state.pendingPiece - 1;
  }
  if (CountPieces(state, player, state.pendingPiece + 1) > 0 &&
      CountPieces(state, player, state.pendingPiece + 2) > 0) {
    return state.pendingPiece;
  }
  return kError;
}
}  // namespace

std::unique_ptr<GameState> Chi(std::unique_ptr<GameState> state) {
  // only gives a single one of the chis
  // ui oof
  const Piece chi_start = GetChiStart(*state, state->lastCaller);
  if (chi_start == kError) {
    std::cerr << "Failed to get start of Chi" << '\n';
    state->nextState = Error;
    return state;
  }

  if (state->hands.at(state->currentPlayer).riichi &&
      state->hands.at(state->currentPlayer).discards.size() ==
          state->hands.at(state->currentPlayer).riichiPieceDiscard) {
    state->hands.at(state->currentPlayer).riichiPieceDiscard++;
  }

  state->hands.at(state->lastCaller).open = true;
  state->currentPlayer = state->lastCaller;

  AlertPlayers(
      *state,
      Event{
          .type = Event::kChi,                                   // type
          .player = state->lastCaller,                           // player
          .piece = static_cast<int16_t>(chi_start.toUint8_t()),  // piece
          .decision = false,                                     // decision
      });

  state->hands.at(state->lastCaller).live.push_back(state->pendingPiece);
  state->hands.at(state->lastCaller).sort();
  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;

  if (RemovePieces(*state, state->lastCaller, chi_start, /*count=*/1) != 1 ||
      RemovePieces(*state, state->lastCaller, chi_start + 1, /*count=*/1) !=
          1 ||
      RemovePieces(*state, state->lastCaller, chi_start + 2, /*count=*/1) !=
          1) {
    std::cerr << "Not Enough Pieces to remove in Chi" << '\n';
    state->nextState = Error;
    return state;
  }
  state->hands.at(state->lastCaller).melds.push_back({Meld::kChi, chi_start});

  state->pendingPiece = AskForDiscard(*state);

  state->nextState = Discard;
  return state;
}

}  // namespace mahjong
