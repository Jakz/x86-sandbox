#pragma once

#include <cstdlib>
#include <stdint.h>
#include <memory>

namespace std
{
  template<typename T, typename ...Args> inline std::unique_ptr<T> make_unique(Args&& ...args) { return std::unique_ptr<T>(new T(std::forward<Args>(args)...)); }
}

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s64 = int64_t;

using reg8 = u8;
using reg16 = u16;
using reg32 = u32;
using reg64 = u64;

using opcode = u8;

class Memory;
class CPU;

struct Machine
{
  class Memory* mem;
  class CPU* cpu;
};