#include "testai.h"
#include "event.h"

auto TesterBot::Name() -> std::string{
  return "TesterBot";
}

auto TesterBot::GameStart(int) -> void {}

auto TesterBot::RoundStart(std::vector<Mahjong::Piece>, Mahjong::Wind, Mahjong::Wind) -> void {
}

auto TesterBot::ReceiveEvent(Mahjong::Event e) -> void{
  events.push_back(e);
}

auto TesterBot::AddEvents(std::vector<Mahjong::Event> e) -> void {
  events.insert(events.end(),e.begin(),e.end());
}

auto TesterBot::GetEvents() -> std::vector<Mahjong::Event> {
  std::vector<Mahjong::Event> e;
  std::swap(events,e);
  return e;
}

auto TesterBot::RetrieveDecision() -> Mahjong::Event{
  if(queue.empty()){
    throw "Not Enough Events";
  }
  Mahjong::Event e = queue.back();
  queue.pop_back();
  return e;
}
