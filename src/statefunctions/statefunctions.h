#pragma once
#include "types/gamestate.h"
#include "types/settings.h"

namespace mahjong {

using StateFunction = GameState& (*)(GameState&);

GameState& GameStart(GameState& state);

GameState& RoundStart(GameState& state);

GameState& Draw(GameState& state);

GameState& PlayerHand(GameState& state);

GameState& Riichi(GameState& state);

GameState& ConcealedKan(GameState& state);

GameState& ConvertedKan(GameState& state);

GameState& Tsumo(GameState& state);

GameState& Discard(GameState& state);

GameState& Pon(GameState& state);

GameState& Chi(GameState& state);

GameState& Kan(GameState& state);

GameState& Exhaust(GameState& state);

GameState& KanDiscard(GameState& state);

GameState& Replacement(GameState& state);

GameState& Ron(GameState& state);

GameState& RoundEnd(GameState& state);

GameState& GameEnd(GameState& state);

GameState& Error(GameState& state);

}  // namespace mahjong
