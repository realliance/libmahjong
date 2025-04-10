#pragma once
#include <functional>
#include <string>
#include <vector>

namespace mahjong {
class PlayerController;
}

namespace mahjong {
using newControllerInst = std::function<PlayerController*()>;

std::vector<std::string> GetAvailableControllers();
newControllerInst GetController(const std::string& controller);
bool RegisterController(newControllerInst newFunc, const std::string& Name);
}  // namespace mahjong
