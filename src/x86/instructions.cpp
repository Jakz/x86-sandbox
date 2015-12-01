#include "instructions.h"

#include "cpu.h"

using namespace std;

template<typename T> const char* registerMnemonic(u8 reg) { return nullptr; }
template<> const char* registerMnemonic<reg8>(u8 reg) { return RegNames8[reg]; }
template<> const char* registerMnemonic<reg16>(u8 reg) { return RegNames16[reg]; }
template<> const char* registerMnemonic<reg32>(u8 reg) { return RegNames32[reg]; }

#pragma mark Mov r8/16/32, imm8/16/32

template<typename Reg>
class MovImmediateR : public Instruction
{
  opcode op;
  Reg& destination;
  Reg value;
  
public:
  MovImmediateR(Machine& machine, opcode op, Reg value) : Instruction(1 + sizeof(Reg)),
  op(op),
  destination(machine.cpu->regs().getReg<Reg>(op & 0x07)),
  value(value)
  {
    this->value = value;
  }
  
  void execute(Machine& machine) override
  {
    destination = value;
  }
  
  std::string mnemonic() override
  {
    return fmt::sprintf("mov %s, %*h", registerMnemonic<Reg>(op & 0x07), sizeof(Reg)*2, value);
  };
};





#pragma mark Decoder


std::unique_ptr<Instruction> Decoder::decode(Machine& machine)
{
  CPU* cpu = machine.cpu;
  CPU::Mode mode = cpu->getMode();
  
  u8 opcode = cpu->fetch<u8>();
  
  // operand size prefix
  if (opcode == 0x66)
  {
    mode = mode == CPU::Mode::BITS16 ? CPU::Mode::BITS32 : CPU::Mode::BITS16;
    opcode = cpu->fetch<u8>();
  }

  // mov r8, imm8
  if ((opcode & ~0x07) == 0xB0)
  {
    u8 imm = machine.cpu->fetch<u8>();
    return make_unique<MovImmediateR<reg8>>(machine, opcode, imm);
  }
  // mov r16/32, imm16/32
  else if ((opcode & ~0x07) == 0xB8)
  {
    if (mode == CPU::Mode::BITS16)
    {
      u16 imm = machine.cpu->fetch<u16>();
      return make_unique<MovImmediateR<reg16>>(machine, opcode, imm);
    }
    else
    {
      u32 imm = machine.cpu->fetch<u32>();
      return make_unique<MovImmediateR<reg32>>(machine, opcode, imm);
    }
  }
  
  
  return nullptr;
};
