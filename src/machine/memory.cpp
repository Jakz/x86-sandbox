#include "memory.h"

u8 Memory::read8(u64 address) const { return data[address]; }
u16 Memory::read16(u64 address) const { return *reinterpret_cast<u16*>(&data[address]); }
u32 Memory::read32(u64 address) const { return *reinterpret_cast<u32*>(&data[address]); }
u64 Memory::read64(u64 address) const { return *reinterpret_cast<u64*>(&data[address]); }

void Memory::write8(u64 address, u8 value) { data[address] = value; }
void Memory::write16(u64 address, u16 value) { *reinterpret_cast<u16*>(&data[address]) = value; }
void Memory::write32(u64 address, u32 value) { *reinterpret_cast<u32*>(&data[address]) = value; }
void Memory::write64(u64 address, u64 value) { *reinterpret_cast<u64*>(&data[address]) = value; }