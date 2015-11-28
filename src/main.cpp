//
//  main.cpp
//  X86Sim
//
//  Created by Jack on 11/27/15.
//  Copyright (c) 2015 Jack. All rights reserved.
//

#include <iostream>

#include <stdint.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using reg8 = u8;
using reg16 = u16;
using reg32 = u32;
using reg64 = u64;

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
  reg64 rip() { return _ip.r64; }
  
  reg16& cs() { return _cs.r16; }
  reg16& ss() { return _ss.r16; }
  reg16& ds() { return _ds.r16; }
  reg16& es() { return _es.r16; }
  reg16& fs() { return _fs.r16; }
  reg16& gs() { return _gs.r16; }
};

class Memory
{
private:
  u8* data;
  
public:
  u8 read8(u64 address);
  u16 read16(u64 address);
  u32 read32(u64 address);
  u64 read64(u64 address);
  
  void write8(u64 address, u8 value);
  void write16(u64 address, u16 value);
  void write32(u64 address, u32 value);
  void write64(u64 address, u64 value);
};

class Cpu
{
private:
  Memory* mem;
  Registers regs;
};


int main(int argc, const char * argv[])
{
  Registers regs;
  
  regs.ax() = 0x1234;
  regs.cl() = regs.ah();
  
  return 0;
}

