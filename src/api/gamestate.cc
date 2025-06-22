#include "gamestate.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <new>
#include "../statefunctions/statecontroller.h"
#include "../statefunctions/statefunctions.h"
#include "types.h"

namespace api {

namespace {

const std::unordered_map<mahjong::GameState::StateFunction, const char*> kStateFunctionNames = {
    {mahjong::GameStart, "GameStart"},
    {mahjong::RoundStart, "RoundStart"},
    {mahjong::RoundEnd, "RoundEnd"},
    {mahjong::Draw, "Draw"},
    {mahjong::PlayerHand, "PlayerHand"},
    {mahjong::Discard, "Discard"},
    {mahjong::Chi, "Chi"},
    {mahjong::Pon, "Pon"},
    {mahjong::Kan, "Kan"},
    {mahjong::ConcealedKan, "ConcealedKan"},
    {mahjong::ConvertedKan, "ConvertedKan"},
    {mahjong::KanDiscard, "KanDiscard"},
    {mahjong::Replacement, "Replacement"},
    {mahjong::Riichi, "Riichi"},
    {mahjong::Tsumo, "Tsumo"},
    {mahjong::Ron, "Ron"},
    {mahjong::Exhaust, "Exhaust"},
    {mahjong::GameEnd, "GameEnd"},
    {mahjong::Error, "Error"},
    {nullptr, "Unknown"}
};

const char* GetStateFunctionName(mahjong::GameState::StateFunction func) {
  auto it = kStateFunctionNames.find(func);
  return (it != kStateFunctionNames.end()) ? it->second : "Unknown";
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
} // namespace

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
  std::unique_ptr<mahjong::GameState> state = mahjong::InitGameState(cpp_settings);
  return state.release();
}

mahjong::GameState* AdvanceGameState(mahjong::GameState* state) {
  std::unique_ptr<mahjong::GameState> new_state = mahjong::AdvanceGameState(std::unique_ptr<mahjong::GameState>(state));
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
  
  // Static array copies per player
  for (int i = 0; i < 4; i++) {
    observed.scores[i] = state->scores[i];
    observed.points[i] = state->players[i].points;
    observed.hasRonned[i] = state->hasRonned[i];
    
    // Hands (up to max hand size)
    const auto& hand = state->hands[i];
    const int hand_size = static_cast<int>(hand.live.size());
    for (int j = 0; j < 14; j++) {
      if (j < hand_size) {
        observed.hands[i][j] = static_cast<CPiece>(hand.live[j].toUint8_t());
      } else {
        // Fill rest of hand with error (may only be final piece ever?)
        observed.hands[i][j] = static_cast<CPiece>(mahjong::Piece::Type::kError);
      }
    }
    
    // Discards
    const auto& discards = state->hands[i].discards;
    observed.playerDiscardCounts[i] = static_cast<int>(discards.size());
    
    if (discards.empty()) {
      observed.players[i] = nullptr;
    } else {
      try {
        observed.players[i] = new CPiece[discards.size()];
        for (size_t j = 0; j < discards.size(); j++) {
          observed.players[i][j] = static_cast<CPiece>(discards[j].toUint8_t());
        }
      } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed for player " << i 
                  << "discards: size=" << discards.size() 
                  << "bytes=" << (discards.size() * sizeof(CPiece))
                  << "error=" << e.what()
                  << "errno=" << errno << " (" << std::strerror(errno) << ")"
                  << '\n';
        
        // Clean up any previously allocated arrays for this observation
        for (int cleanup_player = 0; cleanup_player < i; cleanup_player++) {
          delete[] observed.players[cleanup_player];
          observed.players[cleanup_player] = nullptr;
        }
        
        // Rethrow after cleanup
        throw;
      }
    }
  }
  
  return observed;
}

void FreeObservedGameState(CObservedGameState* observed) {
  if (!observed) {
    return;
  }
  
  // Free discard arrays
  for (auto & player : observed->players) {
    if (player != nullptr) {
      delete[] player;
      player = nullptr;
    }
  }
}

void FreeGameState(mahjong::GameState* state) {
  delete state;
}
}
} // namespace api
