#pragma once

#include "types.h"

mahjong::GameSettings convertGameSettings(const CGameSettings* settings);

extern "C" {

int StartGame(const CGameSettings* settings, int async);
void ExitGame(int game);
CGameState* InitGameState(const CGameSettings* settings);
CGameState* AdvanceGameState(CGameState* state);
void DestroyGameState(CGameState* state);
}
