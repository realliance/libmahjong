#include "mainwindow.h"
#include <gtkmm/application.h>

auto main (int argc, char *argv[]) -> int
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  MainWindow MainWindow;

  //Shows the window and returns when it is closed.
  return app->run(MainWindow);
}
