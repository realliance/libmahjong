#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "controllers/playercontroller.h"

namespace mahjong {
using newControllerInst = std::function<std::unique_ptr<PlayerController>()>;

class ControllerManager {
 public:
  static ControllerManager& Instance() {
    static ControllerManager controller_manager;
    return controller_manager;
  }

  std::unordered_map<std::string, newControllerInst>
  GetAvailableControllersMap() {
    return available_controllers_;
  }
  std::vector<std::string> GetAvailableControllers();
  std::unique_ptr<mahjong::PlayerController> NewController(
      const std::string& controller);
  bool RegisterController(newControllerInst newFunc, const std::string& Name);

 private:
  std::unordered_map<std::string, newControllerInst> available_controllers_;
};

#define REGISTER_PLAYER_CONTROLLER(controller)                          \
  namespace {                                                           \
  bool __registered = ControllerManager::Instance().RegisterController( \
      &controller::New, #controller);                                   \
  }

}  // namespace mahjong
