#include "controllermanager.h"

#include <alphabeticalbot.h>
#include <angrydiscardobot.h>
#include <fasttanyao.h>
#include <gentlemanbot.h>
#include <thricebot.h>
#include <totobot.h>

#include <map>
#include <string>
#include <utility>
#include <vector>

using Mahjong::newControllerInst, Mahjong::PlayerController;

static std::map<std::string, newControllerInst> availableControllers = {
  {"AngryDiscardoBot", []() -> PlayerController* { return new AngryDiscardoBot; }},
  {"Fast Tanyao", []() -> PlayerController* { return new FastTanyao; }},
  {"TotoBot", []() -> PlayerController* { return new TotoBot; }},
  {"GentlemanBot", []() -> PlayerController* { return new GentlemanBot; }},
  {"AlphabeticalBot", []() -> PlayerController* { return new AlphabeticalBot; }},
  {"ThriceBot", []() -> PlayerController* { return new ThriceBot; }},
};

auto Mahjong::GetAvailableControllers() -> std::vector<std::string> {
  std::vector<std::string> names;
  names.reserve(availableControllers.size());
  for (const auto& [name, _] : availableControllers) {
    names.push_back(name);
  }
  return names;
}

auto Mahjong::GetController(const std::string& controller) -> newControllerInst {
  if (availableControllers.contains(controller)) {
    return availableControllers[controller];
  }
  throw "No Such Controller";
}

auto Mahjong::RegisterController(newControllerInst newFunc, const std::string& name) -> bool {
  if (availableControllers.contains(name)) {
    return false;
  }
  availableControllers[name] = std::move(newFunc);
  return true;
}
