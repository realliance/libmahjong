#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/statefunction.h"

namespace mahjong {
namespace {
Piece GetChiStart(const Player& player, Piece p) {
  if (CountPieces(player, p - 2) > 0 && CountPieces(player, p - 1) > 0) {
    return p - 2;
  }
  if (CountPieces(player, p - 1) > 0 && CountPieces(player, p + 1) > 0) {
    return p - 1;
  }
  if (CountPieces(player, p + 1) > 0 && CountPieces(player, p + 2) > 0) {
    return p;
  }
  return kError;
}

std::unique_ptr<GameState> Chi(std::unique_ptr<GameState> state) {
  // only gives a single one of the chis
  // ui oof
  const Piece chi_start =
      GetChiStart(state->players[state->lastCaller], state->pendingPiece);
  if (chi_start == kError) {
    std::cerr << "Failed to get start of Chi" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }

  if (state->players.at(state->currentPlayer).riichi &&
      state->players.at(state->currentPlayer).discards_count ==
          state->players.at(state->currentPlayer).riichiPieceDiscard) {
    state->players.at(state->currentPlayer).riichiPieceDiscard++;
  }

  Player& player = state->players.at(state->lastCaller);

  player.open = true;
  state->currentPlayer = state->lastCaller;

  AlertPlayers(
      *state,
      Event{
          .type = Event::kChi,                                   // type
          .player = state->lastCaller,                           // player
          .piece = static_cast<int16_t>(chi_start.toUint8_t()),  // piece
          .decision = false,                                     // decision
      });

  player.live[player.live_count++] = state->pendingPiece;
  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;

  if (RemovePieces(state->players[state->lastCaller], chi_start, /*count=*/1) !=
          1 ||
      RemovePieces(state->players[state->lastCaller], chi_start + 1,
                   /*count=*/1) != 1 ||
      RemovePieces(state->players[state->lastCaller], chi_start + 2,
                   /*count=*/1) != 1) {
    std::cerr << "Not Enough Pieces to remove in Chi" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kChi,
      .start = chi_start,
  };

  state->pendingPiece = AskForDiscard(*state);

  state->nextState = StateFunctionType::kDiscard;
  return state;
}
}  // namespace

REGISTER_ROUTE(Chi, StateFunctionType::kChi);
}  // namespace mahjong
