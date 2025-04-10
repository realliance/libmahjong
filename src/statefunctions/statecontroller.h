#pragma once

#include "types/settings.h"

namespace mahjong {
int StartGame(const GameSettings& settings, bool async);
void ExitGame(int game);

void StateController(GameSettings settings);

}  // namespace mahjong
