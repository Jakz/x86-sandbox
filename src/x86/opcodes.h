#pragma once

#include <string>

#include "registers.h"
#include "common/format.h"

enum Opcodes : u8
{
  OP_PREFIX_OPERAND = 0x66,
  OP_PREFIX_ADDRESS = 0x67,
  
  OP_AAA = 0x37,

  OP_MOV_IMM8 = 0xB0,
  OP_MOV_IMM1632 = 0xB8,
  OP_MOV_REG_RM = 0x88,

  OP_NOP = 0x90,
};

enum OpMods : u8
{
  OP_WIDTH_BIT = 0x01,
  OP_DIRECTION_BIT = 0x02
};

class SIBByte
{
public:
  enum class Scale : u8
  {
    BY_1 = 0,
    BY_2,
    BY_4,
    BY_8
  };
  
private:
  union
  {
    struct
    {
      RegIndex base : 3;
      RegIndex index : 3;
      Scale scale : 2;
    };
    u8 value;
  };

public:
  SIBByte(u8 value) : value(value) { }
  SIBByte(RegIndex base, RegIndex index, Scale scale) : base(base), index(index), scale(scale) { }
  
  u32 compute(Registers& regs)
  {
    u32 base = regs.getReg32(this->base);
    u32 index = regs.getReg32(this->index);
    u32 scale = 1<<static_cast<std::underlying_type<Scale>::type>(this->scale);
    
    return base + index*scale;
  }
  
  std::string mnemonic()
  {
    return fmt::sprintf("%s + %s*%u", RegNames32[base], RegNames32[index], 1<<static_cast<std::underlying_type<Scale>::type>(this->scale));
  }
};

template<typename Reg, typename Addr>
class ModRMByte
{
private:
  union
  {
    struct
    {
      u8 rm : 3;
      u8 reg : 3;
      u8 mod : 2;
    };
    u8 value;
  };
  
public:
  ModRMByte(u8 value) : value(value) { }
  
  size_t displacementLength() const;
  std::string mnemonicRM();
  std::string mnemonicReg();
  
  Reg& getRMValue(Machine& machine, Addr displacement);
  Reg& getReg(Machine& machine);
};