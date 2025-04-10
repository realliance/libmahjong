#pragma once
#include <functional>
#include <string>
#include <vector>

namespace mahjong {
class PlayerController;
}

#ifndef NO_PYBIND
namespace pybind11 {
class object;
}
#endif

namespace mahjong {
using newControllerInst = std::function<PlayerController*()>;

std::vector<std::string> GetAvailableControllers();
newControllerInst GetController(const std::string& controller);
bool RegisterController(newControllerInst newFunc, const std::string& Name);
#ifndef NO_PYBIND
bool RegisterPythonController(const pybind11::object& pythonController,
                              const std::string& Name);
void UnregisterController(const std::string& Name);
#endif
}  // namespace mahjong
