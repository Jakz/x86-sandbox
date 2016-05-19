#pragma once

class term
{
private:
  bool shouldQuit;
  
public:
  term() : shouldQuit(false) { }
  
  void draw();
  void init();
  void shutdown();
  
  void handleEvents();
  bool shouldExit() { return shouldQuit; }
};