#pragma once

#include "types.h"

namespace api {

mahjong::GameSettings convertGameSettings(const CGameSettings* settings);

extern "C" {

int StartGame(const CGameSettings* settings, int async);
void ExitGame(int game);
mahjong::GameState* InitGameState(const CGameSettings* settings);
mahjong::GameState* AdvanceGameState(mahjong::GameState* state);
void DestroyGameState(mahjong::GameState* state);
}
} // namespace api
