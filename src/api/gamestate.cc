#include <vector>
#include <memory>
#include <algorithm>
#include <new>
#include "statefunctions/statecontroller.h"
#include "types/piecetype.h"
#include "types/statefunction.h"
#include "types.h"
#include "gamestate.h"

namespace api {

namespace {

CStateFunctionType ConvertStateFunctionType(mahjong::StateFunctionType func) {
  switch (func) {
    case mahjong::StateFunctionType::kError:
      return kError;
    case mahjong::StateFunctionType::kGameStart:
      return kGameStart;
    case mahjong::StateFunctionType::kRoundStart:
      return kRoundStart;
    case mahjong::StateFunctionType::kDraw:
      return kDraw;
    case mahjong::StateFunctionType::kPlayerHand:
      return kPlayerHand;
    case mahjong::StateFunctionType::kPon:
      return kPon;
    case mahjong::StateFunctionType::kChi:
      return kChi;
    case mahjong::StateFunctionType::kKan:
      return kKan;
    case mahjong::StateFunctionType::kConcealedKan:
      return kConcealedKan;
    case mahjong::StateFunctionType::kConvertedKan:
      return kConvertedKan;
    case mahjong::StateFunctionType::kKanDiscard:
      return kKanDiscard;
    case mahjong::StateFunctionType::kReplacement:
      return kReplacement;
    case mahjong::StateFunctionType::kRiichi:
      return kRiichi;
    case mahjong::StateFunctionType::kDiscard:
      return kDiscard;
    case mahjong::StateFunctionType::kExhaust:
      return kExhaust;
    case mahjong::StateFunctionType::kRon:
      return kRon;
    case mahjong::StateFunctionType::kTsumo:
      return kTsumo;
    case mahjong::StateFunctionType::kRoundEnd:
      return kRoundEnd;
    case mahjong::StateFunctionType::kGameEnd:
      return kGameEnd;
  }

  // This should never be reached (unless we forget to add a new state function)
  return kError;
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

bool IsValidGameController(const char* controller) {
  return mahjong::ControllerManager::Instance()
      .GetAvailableControllersMap()
      .contains(controller);
}

mahjong::GameState* InitGameState(const CGameSettings* settings) {
  const mahjong::GameSettings cpp_settings = convertGameSettings(settings);
  // Check controllers ahead of time as C++ throws will not be handled correctly by
  // all library consumers (rust)
  for (const auto& controller : cpp_settings.seatControllers) {
    if (!IsValidGameController(controller.c_str())) {
      return nullptr;
    }
  }

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

  // State function enums
  observed.prevState = ConvertStateFunctionType(state->prevState);
  observed.currState = ConvertStateFunctionType(state->currState);
  observed.nextState = ConvertStateFunctionType(state->nextState);

  // Player data
  for (int i = 0; i < 4; i++) {
    observed.scores[i] = state->scores[i];
    observed.points[i] = state->players[i].points;
    observed.hasRonned[i] = state->hasRonned[i];

    // Convert Hand to CHand
    const auto& cpp_hand = state->hands[i];
    CHand& c_hand = observed.hands[i];

    // Live pieces
    const int live_piece_count = static_cast<int>(cpp_hand.live.size());
    c_hand.livePieceCount = std::min(live_piece_count, kMaxLiveHandSize);
    for (int j = 0; j < c_hand.livePieceCount; j++) {
      c_hand.livePieces[j] = static_cast<CPiece>(cpp_hand.live[j].toUint8_t());
    }

    // Melds
    const int meld_count = static_cast<int>(cpp_hand.melds.size());
    c_hand.meldCount = std::min(meld_count, kMaxMeldsPerHand);
    for (int j = 0; j < c_hand.meldCount; j++) {
      c_hand.melds[j].type = static_cast<CMeldType>(cpp_hand.melds[j].type);
      c_hand.melds[j].start =
          static_cast<CPiece>(cpp_hand.melds[j].start.toUint8_t());
    }

    // Discards
    const auto& discards = cpp_hand.discards;
    const int discards_size = static_cast<int>(discards.size());
    c_hand.discardCount = std::min(discards_size, kMaxDiscardsPerPlayer);
    for (auto j = 0; j < c_hand.discardCount; j++) {
      c_hand.discards[j] = static_cast<CPiece>(discards[j].toUint8_t());
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
