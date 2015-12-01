#pragma once

#include <vector>

#include "common.h"

class Memory
{
private:
  u8* data;
  
public:
  Memory(size_t size) : data(new u8[size]) { }
  ~Memory() { delete [] data; }
  
  template<typename T> T read(u64 address) const { return *reinterpret_cast<T*>(&data[address]);  }

  u8 read8(u64 address) const;
  u16 read16(u64 address) const;
  u32 read32(u64 address) const;
  u64 read64(u64 address) const;
  
  void write8(u64 address, u8 value);
  void write16(u64 address, u16 value);
  void write32(u64 address, u32 value);
  void write64(u64 address, u64 value);
  
  void copy(u64 address, const std::vector<u8>& bytes)
  {
    memcpy(&data[address], &bytes[0], bytes.size());
  }
  
};
