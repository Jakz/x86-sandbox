#include "terminal.h"

#include <ncurses.h>
#include <panel.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int width, height;

WINDOW* wregisters;
PANEL* pregisters;

void term::init()
{  
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  
  width = w.ws_row;
  height = w.ws_col;
  
  initscr();
  keypad(stdscr, true);
  noecho();
  cbreak();
  curs_set(0);
  
  wregisters = newwin(10, width, 0, 0);
  
  pregisters = new_panel(wregisters);
  
}

void term::draw()
{
  wclear(wregisters);
  box(wregisters, 0, 0);
  
  mvwprintw(wregisters, 0, 1, "[Registers]");
  
  update_panels();
	doupdate();
}

void term::handleEvents()
{
  int c = getch();
  
  switch (c) {
    case 'q':
    case 'Q':
      shouldQuit = true;
      break;
      
  }
}

void term::shutdown()
{
  endwin();
}