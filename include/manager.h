#pragma once
#include <stdint.h>         // for int16_t
#include <array>            // for array
#include <map>              // for map
#include <string>           // for string
#include <vector>           // for vector
#include "piecetype.h"      // for Piece
#include "statefunction.h"  // for stateFunction
class MahjongAI;  // lines 11-11
struct Event;  // lines 12-12
struct GameState;  // lines 13-13

using newMahjongAiInst = MahjongAI* (*)();

class MahjongGameManager{
public:  
  MahjongGameManager() = delete;
  static auto GetAvailableAIs() -> std::vector<std::string>;
  static auto RegisterAI(newMahjongAiInst newFunc, std::string Name) -> bool;
  static auto StartGame(std::vector<std::string> seatAIs, bool async) -> void;

  static auto GameLoop(std::vector<std::string> seatAIs) -> void;

  static auto RoundStart(GameState& state) -> stateFunction;
  static auto PlayerTurn(GameState& state) -> stateFunction;
  static auto DiscardState(GameState& state) -> stateFunction;
  static auto RoundEnd(GameState& state) -> stateFunction;

  [[noreturn]] static auto ErrorState(const GameState& state, std::string info) -> void;

  static auto RemovePieces(GameState& state, int player, Piece p, uint8_t count) -> uint8_t;
  static auto DiscardPiece(GameState& state, int player, Piece p) -> void;
  static auto MeldPieces(GameState& state, int player, Piece p, EventType type) -> void;

  static auto CanKan(const GameState& state, int player) -> bool;
  static auto CanPon(const GameState& state, int player) -> bool;
  static auto CanChi(const GameState& state, int player) -> bool;
  static auto GetChiStart(const GameState& state, int player) -> Piece;
  static auto CanRon( GameState& state, int player) -> bool;

  static auto CanConvertedKan(const GameState& state) -> bool;
  static auto CanConcealedKan(const GameState& state, Piece p) -> bool;
  static auto CanTsumo(const GameState& state) -> bool;
  static auto CanRiichi(const GameState& state) -> bool;

  static auto ScorePlayers(const GameState& state) -> std::array<int16_t,4>;
  static auto AlertPlayers(const GameState& state, Event e) -> void;
  static auto CountPieces(const GameState& state, int player, Piece p) -> uint8_t;
  static auto ValidateDecision(GameState& state, int player, Event decision, bool inHand, Piece p) -> bool;
private:
  static std::map<std::string,newMahjongAiInst> availableAIs;
};