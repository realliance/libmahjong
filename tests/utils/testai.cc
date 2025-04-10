#include "testai.h"
#include "types/event.h"

std::string TesterBot::Name() {
  return "TesterBot";
}

void TesterBot::GameStart(int) {}

void TesterBot::RoundStart(std::vector<mahjong::Piece>, mahjong::Wind,
                           mahjong::Wind) {}

void TesterBot::ReceiveEvent(mahjong::Event e) {
  events.push_back(e);
}

void TesterBot::AddEvents(std::vector<mahjong::Event> e) {
  events.insert(events.end(), e.begin(), e.end());
}

std::vector<mahjong::Event> TesterBot::GetEvents() {
  std::vector<mahjong::Event> e;
  std::swap(events, e);
  return e;
}

mahjong::Event TesterBot::RetrieveDecision() {
  if (queue.empty()) {
    throw "Not Enough Events";
  }
  mahjong::Event e = queue.back();
  queue.pop_back();
  return e;
}
