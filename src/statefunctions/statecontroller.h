#pragma once

namespace mahjong {
struct GameSettings;
struct GameState;

int StartGame(const GameSettings& settings, bool async);
void ExitGame(int game);

std::unique_ptr<GameState> InitGameState(const GameSettings& settings);
std::unique_ptr<GameState> AdvanceGameState(std::unique_ptr<GameState> state);
void StateController(const GameSettings& settings);

}  // namespace mahjong
