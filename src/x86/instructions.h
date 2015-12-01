#pragma once

#include <string>

#include "common.h"
#include "registers.h"
#include "common/format.h"

class Instruction
{
private:
  size_t length;
  
protected:
  void setLength(size_t i) { length = i; }
  void increaseLength(size_t i) { length += i; }
  
public:
  Instruction(size_t length = 1) : length(length) { }
  virtual void execute(Machine& env) = 0;
  virtual std::string mnemonic() = 0;
  
  size_t getLength() { return length; }
};

class Decoder
{
private:
  

public:
  std::unique_ptr<Instruction> decode(Machine& machine);
};