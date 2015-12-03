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

class RMInstructionBase : public Instruction
{
protected:
  using Instruction::Instruction;
public:
  virtual size_t fetchDisplacement(Machine& machine) = 0;
  virtual bool hasSibByte() const = 0;
};

template<typename Reg, typename Addr>
class RMInstruction : public RMInstructionBase
{
protected:
  ModRMByte<Reg,Addr> modRm;
  Addr displacement;
  bool sourceIsReg;
  std::string name;
  RMInstruction(size_t length, const std::string& name, u8 opcode, u8 rmByte) : RMInstructionBase(length), name(name), modRm(rmByte), sourceIsReg(!(opcode & OP_DIRECTION_BIT))
  {
    increaseLength(modRm.displacementLength());
  }
  
  template<typename T>
  void setDisplacement(T d)
  {
    displacement = static_cast<Addr>(static_cast<typename make_signed<Addr>::type>(static_cast<typename make_signed<T>::type>(d)));
  }
  
public:
  bool hasSibByte() const override { return modRm.hasSibByte(); }
  
  size_t fetchDisplacement(Machine& machine) override
  {
    size_t length = modRm.displacementLength();
    
    switch (length)
    {
      case 0: displacement = 0; break;
      case 1: setDisplacement(machine.cpu->fetch<u8>()); break;
      case 2: setDisplacement(machine.cpu->fetch<u16>()); break;
      case 4: setDisplacement(machine.cpu->fetch<u32>()); break;

    }

    return length;
  }
  
  std::string mnemonic() override
  {
    std::string reg = modRm.mnemonicReg();
    std::string rm = modRm.mnemonicRM();
    
    if (modRm.displacementLength() > 0)
      rm = fmt::sprintf("[%s + %xh]", rm.c_str(), displacement);
    else if (modRm.isDirect())
      rm = fmt::sprintf("%s", rm.c_str());
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
    if (!this->hasSibByte())
    {
      Reg& source = this->sourceIsReg ? this->modRm.getReg(machine) : this->modRm.getRMValue(machine, this->displacement);
      Reg& dest = this->sourceIsReg ? this->modRm.getRMValue(machine, this->displacement) : this->modRm.getReg(machine);
      dest = source;
    }
  }
};





#pragma mark Decoder


std::unique_ptr<Instruction> Decoder::decode(Machine& machine)
{
  CPU* cpu = machine.cpu;
  Mode operandMode = cpu->getMode(), addressMode = cpu->getMode();
  
  u32 startAddress = cpu->regs().eip();
  
  u8 opcode = cpu->fetch<u8>();
  
  Instruction* i = nullptr;
  
  // operand size prefix
  if (opcode == OP_PREFIX_OPERAND)
  {
    operandMode = operandMode == Mode::BITS16 ? Mode::BITS32 : Mode::BITS16;
    opcode = cpu->fetch<u8>();
  }
  
  if (opcode == OP_PREFIX_ADDRESS)
  {
    addressMode = addressMode == Mode::BITS16 ? Mode::BITS32 : Mode::BITS16;
    opcode = cpu->fetch<u8>();
  }

  
  
  
  
  // mov r8, imm8
  if ((opcode & ~OP_REG_MASK) == OP_MOV_IMM8)
  {
    u8 imm = machine.cpu->fetch<u8>();
    i = new MovImmediateR<reg8>(machine, opcode, imm);
  }
  // mov r16/32, imm16/32
  else if ((opcode & ~OP_REG_MASK) == OP_MOV_IMM1632)
  {
    if (operandMode == Mode::BITS16)
    {
      u16 imm = machine.cpu->fetch<u16>();
      i = new MovImmediateR<reg16>(machine, opcode, imm);
    }
    else
    {
      u32 imm = machine.cpu->fetch<u32>();
      i = new MovImmediateR<reg32>(machine, opcode, imm);
    }
  }
  // mov r8/16/32, rm8/16/32
  // mov rm8/16/32, r8/16/32
  else if ((opcode & ~OP_RM_MASK) == OP_MOV_REG_RM)
  {
    bool is8bit = !(opcode & OP_WIDTH_BIT);
    u8 rmByte = cpu->fetch<u8>();
    
    RMInstructionBase* ii = nullptr;

    if (is8bit)
    {
      if (addressMode == Mode::BITS16)
        ii = new MovRegRM<reg8, reg16>(opcode,rmByte);
      else
        ii = new MovRegRM<reg8, reg32>(opcode, rmByte);
    }
    else
    {
      if (addressMode == Mode::BITS16)
        ii = operandMode == Mode::BITS16 ? (RMInstructionBase*)new MovRegRM<reg16, reg16>(opcode,rmByte) : new MovRegRM<reg32, reg16>(opcode,rmByte);
      else if (addressMode == Mode::BITS32)
      {
        bool hasSib = ii->hasSibByte();
        
        ii = operandMode == Mode::BITS16 ? (RMInstructionBase*)new MovRegRM<reg16, reg32>(opcode,rmByte) : new MovRegRM<reg32, reg32>(opcode,rmByte);
      }
    }

    ii->fetchDisplacement(machine);
    i = ii;
  }
  
  // AAA
  else if (opcode == OP_AAA) { i = new AAA(); }
  // NOP
  else if (opcode == OP_NOP) { i = new NOP(); }
  
  
  if (i) i->setStartingAddress(startAddress);
  
  return unique_ptr<Instruction>(i);
};
