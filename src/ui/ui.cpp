#include "ui.h"

#include "common/format.h"

#include "x86/registers.h"

#include <Fl/Fl_Text_Editor.H>
#include <Fl/Fl_Button.h>

using namespace ui;
using namespace std;

MainWindow* MainWindow::instance = nullptr;

using text_field = Fl_Text_Display;

class ui::RegisterGPWidget : public Fl_Group
{
private:
  Fl_Text_Buffer* buff[3];
  
public:
  RegisterGPWidget(int x, int y, const char* l32, const char* l8h, const char* l8l) : Fl_Group(x,y,46,24,"")
  {
    buff[0] = new Fl_Text_Buffer();
    text_field* disp = new text_field(this->x(),this->y(),w(),h(), l32);
    disp->scrollbar_width(0);
    disp->buffer(buff[0]);

    buff[1] = new Fl_Text_Buffer();
    disp = new text_field(this->x()+w(),this->y(),28,24,l8h);
    disp->scrollbar_width(0);
    disp->buffer(buff[1]);
    
    buff[2] = new Fl_Text_Buffer();
    disp = new text_field(this->x()+w()+28,this->y(),28,24,l8l);
    disp->scrollbar_width(0);
    disp->buffer(buff[2]);
  }
  
  void updateValue(const reg32& reg)
  {
    string buffer;
    
    buffer = fmt::sprintf("%04X", reg >> 16);
    buff[0]->text(buffer.c_str());
    
    buffer = fmt::sprintf("%02X", (reg >> 8) & 0xFF);
    buff[1]->text(buffer.c_str());
    
    buffer = fmt::sprintf("%02X", reg & 0xFF);
    buff[2]->text(buffer.c_str());
  }
};

void cbStepButton(Fl_Widget* w, void* data)
{
  ((Fl_Button*)w)->set();
  printf("Antanius\n");
}

MainWindow::MainWindow(const Machine& machine) : Fl_Double_Window(1280, 800, "X86 Sandbox"), machine(machine)
{
  Fl::set_font(FL_HELVETICA, "Consolas");
  
  regs[0] = new RegisterGPWidget(20,20,"EAX","AH","AL");
  regs[1] = new RegisterGPWidget(20,70,"EBX","BH","BL");
  regs[2] = new RegisterGPWidget(20,120,"ECX","CH","CL");
  regs[3] = new RegisterGPWidget(20,170,"EDX","DH","DL");
  
  regs[0]->updateValue(0);
  regs[1]->updateValue(0);
  regs[2]->updateValue(0);
  regs[3]->updateValue(0);
  
  stepButton = new Fl_Button(20, 250, 100, 20, "Step");
  stepButton->when(FL_WHEN_RELEASE_ALWAYS);
  stepButton->type(FL_NORMAL_BUTTON);
  stepButton->callback(cbStepButton, this);


}


int ui::init(const Machine& machine)
{
  Fl::visual(FL_RGB);
  
  MainWindow* window = new MainWindow(machine);
  
  window->end();
  window->show();
  
  return Fl::run();
}