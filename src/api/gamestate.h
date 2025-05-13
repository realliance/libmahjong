#pragma once

#include "types.h"
#include "stdbool.h"

namespace api {

mahjong::GameSettings convertGameSettings(const CGameSettings* settings);

extern "C" {

int StartGame(const CGameSettings* settings, bool async);
void ExitGame(int game);
mahjong::GameState* InitGameState(const CGameSettings* settings);
mahjong::GameState* AdvanceGameState(mahjong::GameState* state);
void FreeGameState(mahjong::GameState* state);
}
} // namespace api
