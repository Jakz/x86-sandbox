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

#include "assembler/assembler.h"

using namespace std;

int main(int argc, const char * argv[])
{
  Memory* memory = new Memory(100000);
  CPU* cpu = new CPU(memory);
  Machine machine = { memory, cpu };
  Decoder decoder;
  
  /*
   mov eax, 1234h
   mov cl, 80h
   mov dx, 5678h
   mov ebx, 80808080h
   */
  size_t index = 0;
  for (int mod = 0; mod < 3; ++mod)
  {
    for (int reg = 0; reg < 8; ++reg)
    {
      for (int rm = 0; rm < 8; ++rm)
      {
        u8 rmbyte = (mod << 6) | (reg << 3) | rm;
        std::vector<u8> code = { 0x66, 0x67, 0x8b, rmbyte };
        memory->copy(index, code);
        index += code.size();
        
        if (rm == OP_RM_SIB_ENABLE)
        {
          for (int j = 0; j < 256; ++j)
          {
            memory->get<u8>(index) = j;
            ++index;
            
            if (j < 256-1)
            {
              memory->copy(index, code);
              index += code.size();
            }
          }
        }
        
        
        if (mod == 0b01)
        {
          memory->get<u8>(index) = 0x71;
          ++index;
        }
        else if (mod == 0b10 || (mod == 0b00 && rm == 0b101))
        {
          memory->get<u32>(index) = 0x99AABBCC;
          index += 4;
        }
      }
    }
  }
  
  
  //std::vector<u8> code = {0x66,0x67,0x8b,0x80,0x78,0x56};
  //memory->copy(0, code);

  /*assembler::Assembler assembler;
  assembler.parseString(
                        "nop\n"
                        "mov al, 0x80\n"
                        "mov bh, -129\n"
                        "mov bx, 0x12345\n"
                        "mov ebx, 0x12345678\n");
  
  assembler.printCode();
  assembler.save("/Users/Jack/Documents/Dev/asm/foo");*/
  
  
  auto i = decoder.decode(machine);
  while (i) {
    //i->execute(machine);
    printf("Executed %02x %s\n",memory->get<u8>(i->getStartingAddress()+3), i->mnemonic().c_str());
    i = decoder.decode(machine);
  }
  
  return 0;
}

