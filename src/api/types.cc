#include "types.h"
#include "controllers/playercontroller.h"
#include "types/gamestate.h"
#include <memory>

extern "C" {
  struct GameStateWrapper {
    std::unique_ptr<mahjong::GameState> state;
  };
    
  struct PlayerControllerWrapper {
    std::unique_ptr<mahjong::PlayerController> controller;
  };
}