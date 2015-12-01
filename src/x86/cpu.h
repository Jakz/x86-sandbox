#pragma once

#include "common.h"
#include "registers.h"
#include "machine/memory.h"

class CPU
{
public:
  enum class Mode
  {
    BITS16,
    BITS32,
    BITS64
  };
  
private:
  Memory* mem;
  Registers r;
  Mode mode;
  
public:
  CPU(Memory* memory) : mem(memory), mode(Mode::BITS32) { }
  
  Mode getMode() const { return mode; }
  
  Registers& regs() { return r; }
  
  template<typename T>
  T fetch() {
    T value = mem->read<T>(r.eip());
    r.eip() += sizeof(T);
    return value;
  }
};