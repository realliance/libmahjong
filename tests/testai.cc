#include "testai.h"
#include "event.h"

auto TesterBot::Name() -> std::string{
  return "TesterBot";
}

auto TesterBot::GameStart(int) -> void {}

auto TesterBot::RoundStart(std::vector<mahjong::Piece>, mahjong::Wind, mahjong::Wind) -> void {
}

auto TesterBot::ReceiveEvent(mahjong::Event e) -> void{
  events.push_back(e);
}

auto TesterBot::AddEvents(std::vector<mahjong::Event> e) -> void {
  events.insert(events.end(),e.begin(),e.end());
}

auto TesterBot::GetEvents() -> std::vector<mahjong::Event> {
  std::vector<mahjong::Event> e;
  std::swap(events,e);
  return e;
}

auto TesterBot::RetrieveDecision() -> mahjong::Event{
  if(queue.empty()){
    throw "Not Enough Events";
  }
  mahjong::Event e = queue.back();
  queue.pop_back();
  return e;
}
