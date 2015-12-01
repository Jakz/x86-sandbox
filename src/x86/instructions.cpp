#include "instructions.h"

#include "cpu.h"
#include "opcodes.h"

using namespace std;

class GenericInstruction : public Instruction
{
protected:
  std::string name;
  std::function<void(Machine&)> lambda;
  
public:
  GenericInstruction(size_t length, const std::string& name, std::function<void(Machine&)>&& lambda) : Instruction(length), lambda(lambda), name(name) { }
  void execute(Machine& machine) override { lambda(machine); }
  std::string mnemonic() override { return name; }
};

template<typename Reg, typename Addr>
class RMInstruction : public Instruction
{
protected:
  ModRMByte<Reg,Addr> modRm;
  Addr displacement;
  bool sourceIsReg;
  std::string name;
  RMInstruction(size_t length, const std::string& name, u8 opcode, u8 rmByte) : Instruction(length), name(name), modRm(rmByte), sourceIsReg(!(opcode & 0x02))
  {
    increaseLength(modRm.displacementLength());
  }
  
public:
  size_t getDisplacement() const { return modRm.displacementLength(); }
  
  template<typename T>
  void setDisplacement(T d)
  {
    displacement = static_cast<Addr>(static_cast<typename make_signed<Addr>::type>(static_cast<typename make_signed<T>::type>(d)));
  }
  
  std::string mnemonic() override
  {
    std::string reg = modRm.mnemonicReg();
    std::string rm = modRm.mnemonicRM();
    
    if (getDisplacement() > 0)
      rm = fmt::sprintf("[%s + %x]", rm.c_str(), displacement);
    else
      rm = fmt::sprintf("[%s]", rm.c_str());
    
    return fmt::sprintf("%s %s, %s", name.c_str(), sourceIsReg ? rm.c_str() : reg.c_str(), sourceIsReg ? reg.c_str() : rm.c_str());
  }
};

#pragma mark AAA

class AAA : public GenericInstruction
{
public:
  AAA() : GenericInstruction(1, "aad", [](Machine& machine) {
    CPU* cpu = machine.cpu;
    Registers& regs = cpu->regs();
    
    if ((regs.al() & 0x0F) > 9 || regs.flags().AF)
    {
      regs.ax() += 0x106;
      regs.flags().AF = 1;
      regs.flags().CF = 1;
    }
    else
    {
      regs.flags().AF = 0;
      regs.flags().CF = 0;
    }
    
    regs.al() &= 0x0F;
  })
  { }
};

#pragma mark NOP

class NOP : public GenericInstruction
{
public:
  NOP() : GenericInstruction(1, "nop", [](Machine&){}) { }
};


#pragma mark MOV r8/16/32, imm8/16/32

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
    return fmt::sprintf("mov %s, %0*xh", registerMnemonic<Reg>(op & 0x07), sizeof(Reg)*2, value);
  };
};

template<typename Reg, typename Addr>
class MovRegRM : public RMInstruction<Reg, Addr>
{
public:
  MovRegRM(opcode op, u8 rmByte) : RMInstruction<Reg, Addr>(2, "mov", op, rmByte) { }
  
  void execute(Machine& machine) override
  {
    Reg& source = this->sourceIsReg ? this->modRm.getReg(machine) : this->modRm.getRMValue(machine, this->displacement);
    Reg& dest = this->sourceIsReg ? this->modRm.getRMValue(machine, this->displacement) : this->modRm.getReg(machine);
    dest = source;
  }
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
  // mov r8/16/32, rm8/16/32
  // mov rm8/16/32, r8/16/32
  else if ((opcode & ~0x03) == 0x88)
  {
    bool is8bit = !(opcode & 0x01);
    
    if (is8bit)
    {
      u8 rmByte = cpu->fetch<u8>();
      
      MovRegRM<reg8, reg16>* i = new MovRegRM<reg8, reg16>(opcode,rmByte);
      
      size_t displacement = i->getDisplacement();
      
      if (displacement == 1)
      {
        u8 d = cpu->fetch<u8>();
        i->setDisplacement(d);
      }
      else if (displacement == 2)
      {
        u16 d = cpu->fetch<u16>();
        i->setDisplacement(d);
      }
      else if (displacement == 4)
      {
        u32 d = cpu->fetch<u32>();
        i->setDisplacement(d);
      }

      return unique_ptr<Instruction>(i);
    }
    
    
  }
  
  // AAA
  else if (opcode == 0x37)
  {
    return make_unique<AAA>();
  }
  // NOP
  else if (opcode == 0x90)
  {
    return make_unique<NOP>();
  }
  
  
  return nullptr;
};
