#pragma once
namespace Mahjong {
struct GameState;
}

namespace Mahjong {

auto CanKan(const GameState& state, int player) -> bool;
auto CanPon(const GameState& state, int player) -> bool;
auto CanChi(const GameState& state, int player) -> bool;
auto CanRon(const GameState& state, int player) -> bool;

auto CanTsumo(const GameState& state) -> bool;
auto CanConcealedKan(const GameState& state) -> bool;
auto CanConvertedKan(const GameState& state) -> bool;
auto CanRiichi(const GameState& state) -> bool;
}  // namespace Mahjong
