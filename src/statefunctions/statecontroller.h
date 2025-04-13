#pragma once

namespace mahjong {
struct GameSettings;

int StartGame(const GameSettings& settings, bool async);
void ExitGame(int game);

void StateController(GameSettings settings);

}  // namespace mahjong
