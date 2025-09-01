#include "controllers/controllermanager.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace mahjong {
class PlayerController;

std::vector<std::string> ControllerManager::GetAvailableControllers() {
  std::vector<std::string> names;
  names.reserve(available_controllers_.size());
  for (const auto& [name, _] : available_controllers_) {
    names.push_back(name);
  }
  return names;
}

std::unique_ptr<mahjong::PlayerController> ControllerManager::NewController(
    const std::string& controller) {
  if (available_controllers_.contains(controller)) {
    return available_controllers_[controller]();
  }
  throw "No Such Controller";
}

bool ControllerManager::RegisterController(newControllerInst newFunc,
                                           const std::string& name) {
  if (available_controllers_.contains(name)) {
    return false;
  }
  available_controllers_[name] = std::move(newFunc);
  return true;
}

}  // namespace mahjong
