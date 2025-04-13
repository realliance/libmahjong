#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "playercontroller.h"

namespace mahjong {
using newControllerInst = std::function<std::unique_ptr<PlayerController>()>;

class ControllerManager {
 public:
  static ControllerManager& Instance() {
    static ControllerManager controller_manager;
    return controller_manager;
  }

  std::vector<std::string> GetAvailableControllers();
  std::unique_ptr<mahjong::PlayerController> NewController(
      const std::string& controller);
  bool RegisterController(newControllerInst newFunc, const std::string& Name);

 private:
  std::map<std::string, newControllerInst> available_controllers_;
};

#define REGISTER_PLAYER_CONTROLLER(controller)                          \
  namespace {                                                           \
  bool __registered = ControllerManager::Instance().RegisterController( \
      &controller::New, #controller);                                   \
  }

}  // namespace mahjong
