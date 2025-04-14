#pragma once
#include <memory>

#include "types/gamestate.h"
#include "types/settings.h"

namespace mahjong {

[[nodiscard]] std::unique_ptr<GameState> GameStart(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> RoundStart(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Draw(std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> PlayerHand(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Riichi(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> ConcealedKan(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> ConvertedKan(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Tsumo(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Discard(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Pon(std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Chi(std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Kan(std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Exhaust(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> KanDiscard(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Replacement(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Ron(std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> RoundEnd(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> GameEnd(
    std::unique_ptr<GameState> state);

[[nodiscard]] std::unique_ptr<GameState> Error(
    std::unique_ptr<GameState> state);

}  // namespace mahjong
