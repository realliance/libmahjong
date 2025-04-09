#include "controllermanager.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "controllers/alphabeticalbot.h"
#include "controllers/angrydiscardobot.h"
#include "controllers/fasttanyao.h"
#include "controllers/gentlemanbot.h"
#include "controllers/thricebot.h"
#include "controllers/totobot.h"

namespace mahjong {

namespace {
std::map<std::string, newControllerInst> available_controllers = {
    {"AngryDiscardoBot",
     []() -> PlayerController* {
       return new AngryDiscardoBot;
     }},
    {"Fast Tanyao",
     []() -> PlayerController* {
       return new FastTanyao;
     }},
    {"TotoBot",
     []() -> PlayerController* {
       return new TotoBot;
     }},
    {"GentlemanBot",
     []() -> PlayerController* {
       return new GentlemanBot;
     }},
    {"AlphabeticalBot",
     []() -> PlayerController* {
       return new AlphabeticalBot;
     }},
    {"ThriceBot",
     []() -> PlayerController* {
       return new ThriceBot;
     }},
};
}  // namespace

auto GetAvailableControllers() -> std::vector<std::string> {
  std::vector<std::string> names;
  names.reserve(available_controllers.size());
  for (const auto& [name, _] : available_controllers) {
    names.push_back(name);
  }
  return names;
}

auto GetController(const std::string& controller) -> newControllerInst {
  if (available_controllers.contains(controller)) {
    return available_controllers[controller];
  }
  throw "No Such Controller";
}

auto RegisterController(newControllerInst newFunc, const std::string& name)
    -> bool {
  if (available_controllers.contains(name)) {
    return false;
  }
  available_controllers[name] = std::move(newFunc);
  return true;
}

}  // namespace mahjong
