#pragma once
#include <functional>
#include <string>
#include <vector>

namespace Mahjong {
class PlayerController;
}

#ifndef NO_PYBIND
namespace pybind11 {
class object;
}
#endif

namespace Mahjong {
using newControllerInst = std::function<PlayerController*()>;

auto GetAvailableControllers() -> std::vector<std::string>;
auto GetController(const std::string& controller) -> newControllerInst;
auto RegisterController(newControllerInst newFunc, const std::string& Name) -> bool;
#ifndef NO_PYBIND
auto RegisterPythonController(const pybind11::object& pythonController, const std::string& Name) -> bool;
auto UnregisterController(const std::string& Name) -> void;
#endif
}  // namespace Mahjong
