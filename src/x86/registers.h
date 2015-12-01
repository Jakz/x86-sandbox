#pragma once

#include "common.h"

struct RegisterGP
{
public:
  union {
    u64 r64;
    struct {
      union {
        u32 r32;
        union {
          u16 p2;
          union {
            u16 r16;
            struct {
              u8 r8l;
              u8 r8h;
            };
          };
        };
      };
      u32 p1;
    };
  };
};

struct RegisterLegacy
{
  u16 r16;
};

struct RegisterFlags
{
  union
  {
    u64 r64;
    struct {
      union {
        u32 r32;
        struct {
          bool CF : 1;
          bool R4 : 1;
          bool PF : 1;
          bool R3 : 1;
          bool AF : 1;
          bool R2 : 1;
          bool ZF : 1;
          bool SF : 1;
          bool TF : 1;
          bool IF : 1;
          bool DF : 1;
          bool OF : 1;
          bool IOPL : 1;
          bool NT : 1;
          bool R1 : 1;
          bool RF : 1;
          bool VM : 1;
          bool AC : 1;
          bool VIF : 1;
          bool VIP : 1;
          bool ID : 1;
          u32 p2 : 10;
        };
      };
      u32 p1;
    };
  };
};

enum RegIndex : u8
{
  AL = 0b000, AX = AL, EAX = AL,
  CL = 0b001, CX = CL, ECX = CL,
  DL = 0b010, DX = DL, EDX = DL,
  BL = 0b011, BX = BL, EBX = BL,
  AH = 0b100, SP = AH, ESP = AH,
  CH = 0b101, BP = CH, EBP = CH,
  DH = 0b110, SI = DH, ESI = DH,
  BH = 0b111, DI = BH, EDI = BH,
  
  REG_NONE = 0xff
};

struct Registers
{
private:
  RegisterGP _a, _b, _c, _d;
  RegisterGP _bp, _si, _di, _sp;
  RegisterLegacy _cs, _ss, _ds, _es, _fs, _gs;
  RegisterGP _ip;
  
  RegisterFlags _flags;
  
public:
  Registers()
  {
    _a.r64 = 0LL;
    _b.r64 = 0LL;
    _c.r64 = 0LL;
    _d.r64 = 0LL;
    
    _bp.r64 = 0LL;
    _si.r64 = 0LL;
    _di.r64 = 0LL;
    _sp.r64 = 0LL;
    
    _cs.r16 = 0LL;
    _ss.r16 = 0LL;
    _ds.r16 = 0LL;
    _es.r16 = 0LL;
    _fs.r16 = 0LL;
    _gs.r16 = 0LL;
    
  }
  
  RegisterFlags& flags() { return _flags; }
  
  reg8& ah() { return _a.r8h; }
  reg8& al() { return _a.r8l; }
  reg16& ax() { return _a.r16; }
  reg32& eax() { return _a.r32; }
  reg64& rax() { return _a.r64; }
  
  reg8& bh() { return _b.r8h; }
  reg8& bl() { return _b.r8l; }
  reg16& bx() { return _b.r16; }
  reg32& ebx() { return _b.r32; }
  reg64& rbx() { return _b.r64; }
  
  reg8& ch() { return _c.r8h; }
  reg8& cl() { return _c.r8l; }
  reg16& cx() { return _c.r16; }
  reg32& ecx() { return _c.r32; }
  reg64& rcx() { return _c.r64; }
  
  reg8& dh() { return _d.r8h; }
  reg8& dl() { return _d.r8l; }
  reg16& dx() { return _d.r16; }
  reg32& edx() { return _d.r32; }
  reg64& rdx() { return _d.r64; }
  
  reg8& bpl() { return _bp.r8l; }
  reg16& bp() { return _bp.r16; }
  reg32& ebp() { return _bp.r32; }
  reg64& rbp() { return _bp.r64; }
  
  reg8& sil() { return _si.r8l; }
  reg16& si() { return _si.r16; }
  reg32& esi() { return _si.r32; }
  reg64& rsi() { return _si.r64; }
  
  reg8& dil() { return _di.r8l; }
  reg16& di() { return _di.r16; }
  reg32& edi() { return _di.r32; }
  reg64& rdi() { return _di.r64; }
  
  reg8& spl() { return _sp.r8l; }
  reg16& sp() { return _sp.r16; }
  reg32& esp() { return _sp.r32; }
  reg64& rsp() { return _sp.r64; }
  
  reg16& ip() { return _ip.r16; }
  reg32& eip() { return _ip.r32; }
  reg64& rip() { return _ip.r64; }
  
  reg16& cs() { return _cs.r16; }
  reg16& ss() { return _ss.r16; }
  reg16& ds() { return _ds.r16; }
  reg16& es() { return _es.r16; }
  reg16& fs() { return _fs.r16; }
  reg16& gs() { return _gs.r16; }
  
  template<typename R> R& getReg(u8 reg);
  
  reg8& getReg8(u8 reg)
  {
    switch (reg)
    {
      case 0b000: return al();
      case 0b001: return cl();
      case 0b010: return dl();
      case 0b011: return bl();
      case 0b100: return ah();
      case 0b101: return ch();
      case 0b110: return dh();
      case 0b111: return bh();
        
      default: return al();
    }
  }
  
  reg16& getReg16(u8 reg)
  {
    switch (reg)
    {
      case 0b000: return ax();
      case 0b001: return cx();
      case 0b010: return dx();
      case 0b011: return bx();
      case 0b100: return sp();
      case 0b101: return bp();
      case 0b110: return si();
      case 0b111: return di();
        
      default: return ax();
    }
  }
  
  reg32& getReg32(u8 reg)
  {
    switch (reg)
    {
      case 0b000: return eax();
      case 0b001: return ecx();
      case 0b010: return edx();
      case 0b011: return ebx();
      case 0b100: return esp();
      case 0b101: return ebp();
      case 0b110: return esi();
      case 0b111: return edi();
        
      default: return eax();
    }
  }
};

template<> inline reg8& Registers::getReg(u8 reg) { return getReg8(reg); }
template<> inline reg16& Registers::getReg(u8 reg) { return getReg16(reg); }
template<> inline reg32& Registers::getReg(u8 reg) { return getReg32(reg); }


static const char* RegNames8[] = { "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH" };
static const char* RegNames16[] = { "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI" };
static const char* RegNames32[] = { "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI" };

template<typename T> inline const char* registerMnemonic(u8 reg) { return nullptr; }
template<> inline const char* registerMnemonic<reg8>(u8 reg) { return RegNames8[reg]; }
template<> inline const char* registerMnemonic<reg16>(u8 reg) { return RegNames16[reg]; }
template<> inline const char* registerMnemonic<reg32>(u8 reg) { return RegNames32[reg]; }