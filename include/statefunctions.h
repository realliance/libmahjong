#pragma once
#include "gamestate.h"
#include "settings.h"

namespace Mahjong {

auto StartGame(const GameSettings& settings, bool async) -> int;
auto ExitGame(int game) -> void;

auto StateController(GameSettings settings) -> void;

using StateFunction = auto (*)(GameState&) -> GameState&;

auto GameStart(GameState& state) -> GameState&;

auto RoundStart(GameState& state) -> GameState&;

auto Draw(GameState& state) -> GameState&;

auto PlayerHand(GameState& state) -> GameState&;

auto Riichi(GameState& state) -> GameState&;

auto ConcealedKan(GameState& state) -> GameState&;

auto ConvertedKan(GameState& state) -> GameState&;

auto Tsumo(GameState& state) -> GameState&;

auto Discard(GameState& state) -> GameState&;

auto Pon(GameState& state) -> GameState&;

auto Chi(GameState& state) -> GameState&;

auto Kan(GameState& state) -> GameState&;

auto Exhaust(GameState& state) -> GameState&;

auto KanDiscard(GameState& state) -> GameState&;

auto Replacement(GameState& state) -> GameState&;

auto Ron(GameState& state) -> GameState&;

auto RoundEnd(GameState& state) -> GameState&;

auto GameEnd(GameState& state) -> GameState&;

auto Error(GameState& state) -> GameState&;

}  // namespace Mahjong
