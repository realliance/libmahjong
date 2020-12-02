#include "controllermanager.h"
#include <angrydiscardobot.h>
#include <thricebot.h>
#include <fasttanyao.h>
#include <alphabeticalbot.h>
#include <totobot.h>
#include <gentlemanbot.h>
#include <map>
#include <string>
#include <vector>
#include "statefunctions.h"
#include "playercontroller.h"
#include "settings.h"

using namespace Mahjong;

static std::map<std::string,newControllerInst> availableControllers = {
  {"AngryDiscardoBot",[]() -> PlayerController* {return new AngryDiscardoBot;}},
  {"Fast Tanyao",[]() -> PlayerController* {return new FastTanyao;}},
  {"TotoBot",[]() -> PlayerController* {return new TotoBot;}},
  {"GentlemanBot",[]() -> PlayerController* {return new GentlemanBot;}},
  {"AlphabeticalBot",[]() -> PlayerController* {return new AlphabeticalBot;}},
  {"ThriceBot",[]() -> PlayerController* {return new ThriceBot;}},
};

auto Mahjong::GetAvailableControllers() -> std::vector<std::string> {
  std::vector<std::string> names;
  for(const auto &[name, _] : availableControllers){
    names.push_back(name);
  }
  return names;
}

auto Mahjong::GetController(std::string controller) -> newControllerInst{
  if(availableControllers.contains(controller)){
    return availableControllers[controller];
  }
  throw "No Such Controller";
}

auto Mahjong::RegisterController(newControllerInst newFunc, std::string name) -> bool {
  if(availableControllers.contains(name)){
    return false;
  }
  availableControllers[name] = newFunc;
  return true;
}
