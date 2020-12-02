#include <iostream>
#include <gdkmm/pixbuf.h>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "analysis.h"
#include "handnode.h"
#include "mainwindow.h"

MainWindow::MainWindow()
: button("Get New Hand"){

  set_border_width(10);

  button.signal_clicked().connect(sigc::mem_fun(*this,
              &MainWindow::on_button_clicked));

  sortButton = Gtk::CheckButton("Sorted");
  sortButton.set_active();
  stopButton = Gtk::CheckButton("Stopped");
  stopButton.set_active(false);
  loopButton = Gtk::CheckButton("Loop");
  loopButton.set_active(false);

  stdformbutton = Gtk::CheckButton("Standard form");
  stdformbutton.set_active(false);

  errorPercent = Gtk::Label("Error rate:");
  heavenPercent = Gtk::Label("Heaven rate:");

  path = "../../build/_deps/riichimahjongtiles-src/Regular";
  auto Hand = walls.TakeHand();
  Hand.push_back(walls.TakePiece());
  std::sort(Hand.begin(),Hand.end());
  isStdForm = Gtk::Label("Not Checked Yet");
  front = Gdk::Pixbuf::create_from_file("../../build/_deps/riichimahjongtiles-src/Regular/Front.svg");
  for(int i = 0; i < 14; i ++){
    int width = 100;
    auto top = Gdk::Pixbuf::create_from_file(getFilePath(Hand.at(i).toUint8_t()));
    auto piece = front->copy();
    top->composite(
      piece,0,0,top->get_width(),top->get_height(),
      150*0.1,
      width*((float)piece->get_width()/piece->get_height())*0.2,
      0.9,0.9,Gdk::INTERP_BILINEAR,255);
    imageArray.emplace_back(Gtk::Image(piece->scale_simple(
      width*((float)piece->get_width()/piece->get_height()),
      width,Gdk::INTERP_BILINEAR )));
    grid.attach(imageArray.at(i),i,0);
  }

  add(grid);
  grid.set_column_spacing(10);
  grid.set_row_spacing(10);
  grid.attach(button,3,1,2);
  grid.attach(sortButton,5,1);
  grid.attach(stdformbutton,8,1,2);
  grid.attach(isStdForm,0,1,3);
  grid.attach(stopButton,6,1,1);
  grid.attach(loopButton,7,1,1);
  grid.attach(errorPercent,10,1,2);
  grid.attach(heavenPercent,12,1,2);
  grid.show_all();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_button_clicked()
{
  if(stopButton.get_active()){
    return;
  }
  std::vector<Mahjong::Piece> Hand;
  int i =0;
  bool notstop = true;
  do{
    if(i % 1000 == 0 && loopButton.get_active()){
      std::cout << "============== " << i << " ==============" << std::endl;
    }
    i++;
    total++;
    if(stdformbutton.get_active()){
      Hand = Mahjong::GetPossibleStdFormHand();
    }else{
      walls = Mahjong::Walls();
      Hand = walls.TakeHand();
      Hand.push_back(walls.TakePiece());
    }
    if(sortButton.get_active()){
      std::sort(Hand.begin(),Hand.end());
    }
    std::string dotsStr = "";
    dots = (dots+1)%4;
    if(dots == 4){
      dots = 0;
    }
    if(dots == 1){
      dotsStr = ".";
    }
    if(dots == 2){
      dotsStr = "..";
    }
    if(dots == 3){
      dotsStr = "...";
    }
    auto root = breakdownHand(Hand);
    bool stdform = true;
    int cnt = 0;
    bool branched = false;
    bool newbranch = false;
    int nn = 0;
    for(const auto & n : *root){
      if(newbranch){
        newbranch = false;
        stdform = true;
      }
      if(n.type == Mahjong::Node::Single || n.type == Mahjong::Node::Error){
        stdform = false;
        if(!branched){
          break;
        }
      }
      if(n.leaves.size() > 1){
        nn++;
        if(nn > 5){
          std::cout << "stopped at:=== " << i << " ==============" << std::endl;
          notstop = false;
        }
        branched = true;
      }
      cnt++;
      if(n.leaves.empty()){
        if(stdform){
          break;
        }
        newbranch = true;
      }
    }
    if(stdform && cnt > 4){
      isStdForm.set_text("In Standard Form"+dotsStr);
      if(!stdformbutton.get_active()){
        std::cout << "stopped at:=== " << i << " ==============" << std::endl;
        heavenRate++;
        isStdForm.set_text("Blessing of Heaven"+dotsStr);
        notstop = false;
      }
    }else {
      isStdForm.set_text("Not Standard Form"+dotsStr);
      if(stdformbutton.get_active()){
        std::cout << "stopped at:=== " << i << " ==============" << std::endl;
        errorRate++;
        isStdForm.set_text("Error Case Found"+dotsStr);
        stopButton.set_active();
      }
    }
    if(!(notstop && loopButton.get_active() && i< 1000000 && !stopButton.get_active())){
      std::ofstream os("hand.gv");
      root->DumpAsDot(os);
      os.close();
    }
    
  }while(notstop && loopButton.get_active() && i< 1000000 && !stopButton.get_active());
  std::stringstream err;
  err << (double)errorRate/total;
  errorPercent.set_text("Error rate: " + err.str());
  std::stringstream heaven;
  heaven << (double)heavenRate/total;
  heavenPercent.set_text("Heaven rate: " + heaven.str());
  for(int i = 0; i < 14; i ++){
    int width = 100;
    auto top = Gdk::Pixbuf::create_from_file(getFilePath(Hand.at(i).toUint8_t()));
    auto piece = front->copy();
    top->composite(
      piece,0,0,top->get_width(),top->get_height(),
      0,0,1,1,Gdk::INTERP_BILINEAR,255);
    imageArray.at(i).set(
      piece->scale_simple(
        width*((float)piece->get_width()/piece->get_height()),
        width,Gdk::INTERP_BILINEAR
      )
    );
  }
}

const std::map<uint8_t,std::string> FILE_MAP = {
    {Mahjong::Piece::RED_DRAGON , "Chun.svg"},
    {Mahjong::Piece::WHITE_DRAGON , "Haku.svg"},
    {Mahjong::Piece::GREEN_DRAGON , "Hatsu.svg"},
    {Mahjong::Piece::ONE_CHARACTER , "Man1.svg"},
    {Mahjong::Piece::TWO_CHARACTER , "Man2.svg"},
    {Mahjong::Piece::THREE_CHARACTER , "Man3.svg"},
    {Mahjong::Piece::FOUR_CHARACTER , "Man4.svg"},
    {Mahjong::Piece::RED_FIVE_CHARACTER , "Man5-Dora.svg"},
    {Mahjong::Piece::FIVE_CHARACTER , "Man5.svg"},
    {Mahjong::Piece::SIX_CHARACTER , "Man6.svg"},
    {Mahjong::Piece::SEVEN_CHARACTER , "Man7.svg"},
    {Mahjong::Piece::EIGHT_CHARACTER , "Man8.svg"},
    {Mahjong::Piece::NINE_CHARACTER , "Man9.svg"},
    {Mahjong::Piece::SOUTH_WIND , "Nan.svg"},
    {Mahjong::Piece::NORTH_WIND , "Pei.svg"},
    {Mahjong::Piece::ONE_PIN , "Pin1.svg"},
    {Mahjong::Piece::TWO_PIN , "Pin2.svg"},
    {Mahjong::Piece::THREE_PIN , "Pin3.svg"},
    {Mahjong::Piece::FOUR_PIN , "Pin4.svg"},
    {Mahjong::Piece::RED_FIVE_PIN , "Pin5-Dora.svg"},
    {Mahjong::Piece::FIVE_PIN , "Pin5.svg"},
    {Mahjong::Piece::SIX_PIN , "Pin6.svg"},
    {Mahjong::Piece::SEVEN_PIN , "Pin7.svg"},
    {Mahjong::Piece::EIGHT_PIN , "Pin8.svg"},
    {Mahjong::Piece::NINE_PIN , "Pin9.svg"},
    {Mahjong::Piece::WEST_WIND , "Shaa.svg"},
    {Mahjong::Piece::ONE_BAMBOO , "Sou1.svg"},
    {Mahjong::Piece::TWO_BAMBOO , "Sou2.svg"},
    {Mahjong::Piece::THREE_BAMBOO , "Sou3.svg"},
    {Mahjong::Piece::FOUR_BAMBOO , "Sou4.svg"},
    {Mahjong::Piece::RED_FIVE_BAMBOO , "Sou5-Dora.svg"},
    {Mahjong::Piece::FIVE_BAMBOO , "Sou5.svg"},
    {Mahjong::Piece::SIX_BAMBOO , "Sou6.svg"},
    {Mahjong::Piece::SEVEN_BAMBOO , "Sou7.svg"},
    {Mahjong::Piece::EIGHT_BAMBOO , "Sou8.svg"},
    {Mahjong::Piece::NINE_BAMBOO , "Sou9.svg"},
    {Mahjong::Piece::EAST_WIND , "Ton.svg"},
};

auto MainWindow::getFilePath(Mahjong::Piece p) -> std::string{
 return path + "/" + (FILE_MAP.contains(p.raw_value()) ? FILE_MAP.at(p.raw_value()) : "Back.svg");
}
