#pragma once

#include "common.h"
#include "registers.h"
#include "machine/memory.h"

enum class Mode
{
  BITS8,
  BITS16,
  BITS32,
  BITS64
};

class CPU
{
public:

  
private:
  Memory* mem;
  Registers r;
  Mode mode;
  
public:
  CPU(Memory* memory) : mem(memory), mode(Mode::BITS16) { }
  
  Mode getMode() const { return mode; }
  
  Registers& regs() { return r; }
  
  template<typename T>
  T fetch() {
    T value = mem->read<T>(r.eip());
    r.eip() += sizeof(T);
    return value;
  }
};