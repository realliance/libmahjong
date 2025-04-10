#pragma once
namespace mahjong {
struct GameState;
bool CanKan(const GameState& state, int player);
bool CanPon(const GameState& state, int player);
bool CanChi(const GameState& state, int player);
bool CanRon(const GameState& state, int player);

bool CanTsumo(const GameState& state);
bool CanConcealedKan(const GameState& state);
bool CanConvertedKan(const GameState& state);
bool CanRiichi(const GameState& state);
}  // namespace mahjong
