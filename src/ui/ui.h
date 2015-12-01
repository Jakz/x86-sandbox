#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Display.H>


namespace ui
{
  constexpr size_t WIDTH = 1280;
  constexpr size_t HEIGHT = 800;
  
  class MainWindow : public Fl_Double_Window
  {
    
  public:
    MainWindow() : Fl_Double_Window(1280, 800, "X86 Sandbox")
    {
      Fl::set_font(FL_HELVETICA, "Consolas");
      
      Fl_Text_Buffer* buff = new Fl_Text_Buffer();
      buff->text("EAX: 0x12345678\nEBX: 0x6789ABCD");
      Fl_Text_Display* disp = new Fl_Text_Display(20,20,160,48);
      disp->buffer(buff);
    }
    
    
    static MainWindow* instance;
    
  };
  
  
  
  int init();
}