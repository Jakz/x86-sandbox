#include "opcodes.h"

#include "cpu.h"

static const char* rm16[] = {
  "BX+SI",
  "BX+DI",
  "BP+SI",
  "BP+DI",
  "SI",
  "DI",
  "BP",
  "BX"
};

template<typename Reg, typename Addr>
size_t ModRMByte<Reg,Addr>::displacementLength() const
{
  if (mod == 0b11) return 0;
  else if (mod == 0b01) return sizeof(reg8);
  else if (mod == 0b10) return sizeof(Addr);
  else if (mod == 0b00)
  {
    if (rm == 0b110) return sizeof(Addr);
    else
      return 0;
  }
  
  return 0;
}

template<typename Reg, typename Addr>
Reg& ModRMByte<Reg,Addr>::getRMValue(Machine& machine, Addr displacement)
{
  Registers& regs = machine.cpu->regs();
  
  if (mod != 0b11)
  {
    Addr address;
    reg16& segment = regs.ds();
    
    switch (rm) {
      case 0b000: address = regs.bx() + regs.si(); break;
      case 0b001: address = regs.bx() + regs.di(); break;
      case 0b010: address = regs.bp() + regs.si(); break;
      case 0b011: address = regs.bp() + regs.di(); break;
      case 0b100: address = regs.si(); break;
      case 0b101: address = regs.di(); break;
      case 0b110:
      {
        if (mod == 0b00)
          address = 0;
        else
        {
          address = regs.bp();
          segment = regs.ss();
        }
      }
      case 0b111: address = regs.bx(); break;
      default: break;
    }
    
    address += displacement;
    
    // cast to u16 required to manage overflow
    // TODO: think about A20 gate
    return machine.mem->get<Reg>((u16)((segment << 4) + address));
  }
  else
  {
    return regs.getReg<Reg>(rm);
  }
}

template<typename Reg, typename Addr>
Reg& ModRMByte<Reg,Addr>::getReg(Machine& machine)
{
  return machine.cpu->regs().getReg<Reg>(reg);
}

template<typename Reg, typename Addr>
std::string ModRMByte<Reg,Addr>::mnemonicRM()
{
  if (mod != 0b11)
  {
    if (mod == 0b00 && rm == 0b110) return "";
    else return rm16[rm];
  }
  else
    return registerMnemonic<Reg>(rm);
}

template<typename Reg, typename Addr>
std::string ModRMByte<Reg,Addr>::mnemonicReg()
{
  return registerMnemonic<Reg>(reg);
}

template class ModRMByte<reg16, u16>;
template class ModRMByte<reg8, u16>;