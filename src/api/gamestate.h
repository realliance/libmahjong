#pragma once

#include "stdbool.h"
#include "types.h"

namespace api {

extern "C" {

int StartGame(const CGameSettings* settings, bool async);
void ExitGame(int game);
mahjong::GameState* InitGameState(const CGameSettings* settings);
mahjong::GameState* AdvanceGameState(mahjong::GameState* state);
CObservedGameState ObserveGameState(mahjong::GameState* state);
void FreeGameState(mahjong::GameState* state);
}
}  // namespace api
