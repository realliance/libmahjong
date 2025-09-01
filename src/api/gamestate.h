#pragma once

#include "api/types.h"

namespace mahjong {
struct GameState;
}  // namespace mahjong

namespace api {

extern "C" {

#include <stdbool.h>

int StartGame(const CGameSettings* settings, bool async);
void ExitGame(int game);
bool IsValidGameController(const char* controller);
mahjong::GameState* InitGameState(const CGameSettings* settings);
mahjong::GameState* AdvanceGameState(mahjong::GameState* state);
CObservedGameState ObserveGameState(mahjong::GameState* state);
void FreeGameState(mahjong::GameState* state);
}
}  // namespace api
