#pragma once
#include <functional>
#include <string>
#include <vector>

#include "playercontroller.h"

namespace mahjong {
using newControllerInst = std::function<PlayerController*()>;

std::vector<std::string> GetAvailableControllers();
newControllerInst GetController(const std::string& controller);
bool RegisterController(newControllerInst newFunc, const std::string& Name);
}  // namespace mahjong
