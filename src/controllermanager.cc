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

std::vector<std::string> GetAvailableControllers() {
  std::vector<std::string> names;
  names.reserve(available_controllers.size());
  for (const auto& [name, _] : available_controllers) {
    names.push_back(name);
  }
  return names;
}

newControllerInst GetController(const std::string& controller) {
  if (available_controllers.contains(controller)) {
    return available_controllers[controller];
  }
  throw "No Such Controller";
}

bool RegisterController(newControllerInst newFunc, const std::string& name) {
  if (available_controllers.contains(name)) {
    return false;
  }
  available_controllers[name] = std::move(newFunc);
  return true;
}

}  // namespace mahjong
