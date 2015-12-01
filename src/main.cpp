//
//  main.cpp
//  X86Sim
//
//  Created by Jack on 11/27/15.
//  Copyright (c) 2015 Jack. All rights reserved.
//

#include <iostream>

#include "common.h"
#include "x86/opcodes.h"
#include "x86/registers.h"
#include "x86/instructions.h"

#include "x86/cpu.h"

#include "machine/memory.h"

#include "ui/ui.h"

using namespace std;

int main(int argc, const char * argv[])
{
  Memory* memory = new Memory(1000);
  CPU* cpu = new CPU(memory);
  Machine machine = { memory, cpu };
  Decoder decoder;

  /*
   mov eax, 1234h
   mov cl, 80h
   mov dx, 5678h
   mov ebx, 80808080h
   */
  std::vector<u8> code = {0xB8,0x34,0x12,0x00,0x00,0xB1,0x80,0x66,0xBA,0x78,0x56,0xBB,0x80,0x00,0x00,0x00};
  memory->copy(0, code);
  
  std::vector<u8> code2 = {0x88,0x37,0x88,0x57,0x01};
  memory->copy(code.size(), code2);
  
  
  auto i = decoder.decode(machine);
  while (i) {
    i->execute(machine);
    cout << "Executed " << i->mnemonic() << endl;
    i = decoder.decode(machine);
  }
  
  return 0;
}

