#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Display.H>

class Machine;

class Fl_Button;

namespace ui
{
  constexpr size_t WIDTH = 1280;
  constexpr size_t HEIGHT = 800;
  
  class RegisterGPWidget;
  
  class MainWindow : public Fl_Double_Window
  {
  private:
    const Machine& machine;
    RegisterGPWidget* regs[4];
    
    Fl_Button* stepButton;
    
    
  public:
    MainWindow(const Machine& machine);
    
    static MainWindow* instance;
    
  };
  
  
  
  int init(const Machine& machine);
}