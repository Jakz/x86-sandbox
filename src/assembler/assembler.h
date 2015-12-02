//
//  Compiler.h
//  JJXScriptEngine
//
//  Created by Jack on 9/6/15.
//  Copyright (c) 2015 Jack. All rights reserved.
//

#pragma once

#include "common.h"
#include "x86parser.hpp"
#include "x86lexer.h"
#include "location.hh"
#include "x86/cpu.h"

#include <array>
#include <vector>

namespace assembler
{
  class Assembler
  {
  private:
    Mode mode;
    std::vector<u8> code;
    
  public:
    Assembler();
             
    std::string file;
    
    void warning(const location& l, const std::string& m);
    void error(const location& l, const std::string& m);
    void error(const std::string& m);
    
    void assemble(const std::string& text);
    
    bool parseString(const std::string& text);
    
    void save(const std::string& text);
    
    void printCode();
    
    void emitOperandPrefix(Mode operand);
    void emitAddressPrefix(Mode address);
    void emitModePrefix(Mode operand, Mode address);

    template<typename... Args> void emit(Args... args)
    {
      std::array<u8, sizeof...(Args)> data = {{ static_cast<u8>(args)... }};
      std::copy(std::begin(data), std::end(data), std::back_inserter(code));
    }
    
    template<typename T> void emitValue(T value) { }
  };
  
  template<> inline void Assembler::emitValue<u8>(u8 value) { code.push_back(value); }
  template<> inline void Assembler::emitValue<u16>(u16 value) { code.push_back(value & 0xFF); code.push_back((value >> 8) & 0xFF); }
  template<> inline void Assembler::emitValue<u32>(u32 value) {
    code.push_back(value & 0xFF);
    code.push_back((value >> 8) & 0xFF);
    code.push_back((value >> 16) & 0xFF);
    code.push_back((value >> 24) & 0xFF);
  }
}

