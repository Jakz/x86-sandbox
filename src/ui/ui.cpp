#include "ui.h"

using namespace ui;

MainWindow* MainWindow::instance = nullptr;


int ui::init()
{
  Fl::visual(FL_RGB);
  
  MainWindow* window = new MainWindow();
  
  window->end();
  window->show();
  
  return Fl::run();
}