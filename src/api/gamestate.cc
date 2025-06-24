#include <vector>
#include <memory>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cerrno>
#include <new>
#include "statefunctions/statecontroller.h"
#include "types/piecetype.h"
#include "types/statefunction.h"
#include "types.h"
#include "gamestate.h"

namespace api {

namespace {

const char* GetStateFunctionName(mahjong::StateFunctionType func) {
  switch (func) {
    case mahjong::StateFunctionType::kGameStart:
      return "GameStart";
    case mahjong::StateFunctionType::kRoundStart:
      return "RoundStart";
    case mahjong::StateFunctionType::kRoundEnd:
      return "RoundEnd";
    case mahjong::StateFunctionType::kDraw:
      return "Draw";
    case mahjong::StateFunctionType::kPlayerHand:
      return "PlayerHand";
    case mahjong::StateFunctionType::kDiscard:
      return "Discard";
    case mahjong::StateFunctionType::kChi:
      return "Chi";
    case mahjong::StateFunctionType::kPon:
      return "Pon";
    case mahjong::StateFunctionType::kKan:
      return "Kan";
    case mahjong::StateFunctionType::kConcealedKan:
      return "ConcealedKan";
    case mahjong::StateFunctionType::kConvertedKan:
      return "ConvertedKan";
    case mahjong::StateFunctionType::kKanDiscard:
      return "KanDiscard";
    case mahjong::StateFunctionType::kReplacement:
      return "Replacement";
    case mahjong::StateFunctionType::kRiichi:
      return "Riichi";
    case mahjong::StateFunctionType::kTsumo:
      return "Tsumo";
    case mahjong::StateFunctionType::kRon:
      return "Ron";
    case mahjong::StateFunctionType::kExhaust:
      return "Exhaust";
    case mahjong::StateFunctionType::kGameEnd:
      return "GameEnd";
    case mahjong::StateFunctionType::kError:
      return "Error";
  }
  
  // This should never be reached (unless we forget to add a new state function)
  return "Unknown";
}

mahjong::GameSettings convertGameSettings(const CGameSettings* settings) {
  mahjong::GameSettings cpp_settings;

  cpp_settings.seed = settings->seed;

  // Char arrays to vector
  for (int i = 0; i < settings->num_controllers; i++) {
    if (settings->seat_controllers[i]) {
      cpp_settings.seatControllers.emplace_back(settings->seat_controllers[i]);
    }
  }

  return cpp_settings;
}
}  // namespace

extern "C" {

int StartGame(const CGameSettings* settings, bool async) {
  const mahjong::GameSettings cpp_settings = convertGameSettings(settings);
  return mahjong::StartGame(cpp_settings, async);
}

void ExitGame(int game) {
  mahjong::ExitGame(game);
}

mahjong::GameState* InitGameState(const CGameSettings* settings) {
  const mahjong::GameSettings cpp_settings = convertGameSettings(settings);
  std::unique_ptr<mahjong::GameState> state =
      mahjong::InitGameState(cpp_settings);
  return state.release();
}

mahjong::GameState* AdvanceGameState(mahjong::GameState* state) {
  std::unique_ptr<mahjong::GameState> new_state =
      mahjong::AdvanceGameState(std::unique_ptr<mahjong::GameState>(state));
  return new_state.release();
}

CObservedGameState ObserveGameState(mahjong::GameState* state) {
  CObservedGameState observed = {};
  
  if (!state) {
    return observed;
  }
  
  // Straightforward copies
  observed.currentPlayer = state->currentPlayer;
  observed.turnNum = state->turnNum;
  observed.roundNum = state->roundNum;
  observed.riichiSticks = state->riichiSticks;
  observed.counters = state->counters;
  observed.lastCall = state->lastCall;
  observed.lastCaller = state->lastCaller;
  observed.concealedKan = state->concealedKan;
  observed.seed = state->seed;
  observed.pendingPiece = static_cast<CPiece>(state->pendingPiece.toUint8_t());
  
  // State function names
  observed.prevState = GetStateFunctionName(state->prevState);
  observed.currState = GetStateFunctionName(state->currState);
  observed.nextState = GetStateFunctionName(state->nextState);
  
  // Player data
  for (int i = 0; i < 4; i++) {
    observed.scores[i] = state->scores[i];
    observed.points[i] = state->players[i].points;
    observed.hasRonned[i] = state->hasRonned[i];
    
    // Convert Hand to CHand
    const auto& cpp_hand = state->hands[i];
    CHand& c_hand = observed.hands[i];
    
    // Live pieces
    c_hand.liveCount = static_cast<int>(cpp_hand.live.size());
    for (int j = 0; j < kMaxLiveHandSize; j++) {
      if (j < c_hand.liveCount) {
        c_hand.live[j] = static_cast<CPiece>(cpp_hand.live[j].toUint8_t());
      } else {
        c_hand.live[j] = static_cast<CPiece>(mahjong::Piece::Type::kError);
      }
    }
    
    // Melds
    c_hand.meldCount = static_cast<int>(cpp_hand.melds.size());
    for (int j = 0; j < kMaxMeldsPerHand; j++) {
      if (j < c_hand.meldCount) {
        c_hand.melds[j].type = static_cast<CMeldType>(cpp_hand.melds[j].type);
        c_hand.melds[j].start = static_cast<CPiece>(cpp_hand.melds[j].start.toUint8_t());
      } else {
        c_hand.melds[j].type = kNone;
        c_hand.melds[j].start = static_cast<CPiece>(mahjong::Piece::Type::kError);
      }
    }
    
    // Discards
    const auto& discards = cpp_hand.discards;
    const int discards_size = static_cast<int>(discards.size());
    c_hand.discardCount = std::min(discards_size, kMaxDiscardsPerPlayer);
    for (auto j = 0; j < c_hand.discardCount; j++) {
      c_hand.discards[j] = static_cast<CPiece>(discards[j].toUint8_t());
    }

    // Fill remaining discards with error
    for (auto j = c_hand.discardCount; j < kMaxDiscardsPerPlayer; j++) {
      c_hand.discards[j] = static_cast<CPiece>(mahjong::Piece::Type::kError);
    }
    
    // Hand properties
    c_hand.open = cpp_hand.open;
    c_hand.riichi = cpp_hand.riichi;
    c_hand.riichiPieceDiscard = static_cast<int>(cpp_hand.riichiPieceDiscard);
    c_hand.riichiRound = cpp_hand.riichiRound;
  }
  
  return observed;
}

void FreeGameState(mahjong::GameState* state) {
  delete state;
}
}
}  // namespace api
