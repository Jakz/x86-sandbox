#include "terminal.h"

#include <ncurses.h>
#include <panel.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int width, height;

void term::init()
{
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  
  width = w.ws_row;
  height = w.ws_col;
}