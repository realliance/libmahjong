#pragma once

#include <functional>
#include <memory>

namespace mahjong {
struct GameState;
using StateFunctionFunc =
    std::function<std::unique_ptr<GameState>(std::unique_ptr<GameState>)>;

enum class StateFunctionType {
  kError,
  kGameStart,
  kRoundStart,
  kDraw,
  kPlayerPlayer,
  kPon,
  kChi,
  kKan,
  kConcealedKan,
  kConvertedKan,
  kKanDiscard,
  kReplacement,
  kRiichi,
  kDiscard,
  kExhaust,
  kRon,
  kTsumo,
  kRoundEnd,
  kGameEnd,
};
}  // namespace mahjong
