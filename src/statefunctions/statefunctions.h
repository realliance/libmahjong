#pragma once
#include "types/gamestate.h"
#include "types/settings.h"

namespace mahjong {

[[nodiscard]] GameState&& GameStart(GameState&& state);

[[nodiscard]] GameState&& RoundStart(GameState&& state);

[[nodiscard]] GameState&& Draw(GameState&& state);

[[nodiscard]] GameState&& PlayerHand(GameState&& state);

[[nodiscard]] GameState&& Riichi(GameState&& state);

[[nodiscard]] GameState&& ConcealedKan(GameState&& state);

[[nodiscard]] GameState&& ConvertedKan(GameState&& state);

[[nodiscard]] GameState&& Tsumo(GameState&& state);

[[nodiscard]] GameState&& Discard(GameState&& state);

[[nodiscard]] GameState&& Pon(GameState&& state);

[[nodiscard]] GameState&& Chi(GameState&& state);

[[nodiscard]] GameState&& Kan(GameState&& state);

[[nodiscard]] GameState&& Exhaust(GameState&& state);

[[nodiscard]] GameState&& KanDiscard(GameState&& state);

[[nodiscard]] GameState&& Replacement(GameState&& state);

[[nodiscard]] GameState&& Ron(GameState&& state);

[[nodiscard]] GameState&& RoundEnd(GameState&& state);

[[nodiscard]] GameState&& GameEnd(GameState&& state);

[[nodiscard]] GameState&& Error(GameState&& state);

}  // namespace mahjong
