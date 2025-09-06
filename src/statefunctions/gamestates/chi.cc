#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/statefunction.h"

namespace mahjong {
namespace {
Piece GetChiStart(const Hand& hand, Piece piece) {
  if (CountPieces(hand, piece - 2) > 0 && CountPieces(hand, piece - 1) > 0) {
    return piece - 2;
  }
  if (CountPieces(hand, piece - 1) > 0 && CountPieces(hand, piece + 1) > 0) {
    return piece - 1;
  }
  if (CountPieces(hand, piece + 1) > 0 && CountPieces(hand, piece + 2) > 0) {
    return piece;
  }
  return kError;
}

std::unique_ptr<GameState> Chi(std::unique_ptr<GameState> state) {
  if (Hand& player = state->players[state->currentPlayer];
      player.riichi && player.discards_count == player.riichiPieceDiscard) {
    player.riichiPieceDiscard++;
  }

  state->currentPlayer = state->lastCaller;
  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;

  Hand& player = state->players[state->currentPlayer];
  player.open = true;
  // only gives a single one of the chis
  // ui oof
  const Piece chi_start = GetChiStart(player, state->pendingPiece);
  if (chi_start == kError) {
    std::cerr << "Failed to get start of Chi" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }

  for (int i = 0; i < 3; ++i) {
    if (chi_start + i == state->pendingPiece) {
      continue;
    }
    if (RemovePieces(player, chi_start + i,
                     /*count=*/1) != 1) {
      std::cerr << "Not Enough Pieces to remove in Chi" << '\n';
      state->nextState = StateFunctionType::kError;
      return state;
    }
  }

  player.melds[player.meld_count++] = Meld{
      .type = SetType::kChi,
      .start = chi_start,
  };

  AlertPlayers(
      *state,
      Event{
          .type = Event::kChi,                                   // type
          .player = state->lastCaller,                           // player
          .piece = static_cast<int16_t>(chi_start.toUint8_t()),  // piece
          .decision = false,                                     // decision
      });

  state->pendingPiece = AskForDiscard(*state);

  state->nextState = StateFunctionType::kDiscard;
  return state;
}
}  // namespace

REGISTER_ROUTE(Chi, StateFunctionType::kChi);
}  // namespace mahjong
