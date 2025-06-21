#pragma once

#include "types.h"
#include "stdbool.h"

namespace api {

extern "C" {

int StartGame(const CGameSettings* settings, bool async);
void ExitGame(int game);
mahjong::GameState* InitGameState(const CGameSettings* settings);
mahjong::GameState* AdvanceGameState(mahjong::GameState* state);
CObservedGameState ObserveGameState(mahjong::GameState* state);
void FreeObservedGameState(CObservedGameState* observed);
void FreeGameState(mahjong::GameState* state);
}
} // namespace api
