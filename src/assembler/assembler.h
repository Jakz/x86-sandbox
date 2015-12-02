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

namespace assembler
{
  class Assembler
  {
  private:
    
  public:
    Assembler();
             
    std::string file;
    
    void error(const location& l, const std::string& m);
    void error(const std::string& m);
    
    void assemble(const std::string& text);
    
    bool parseString(const std::string& text);
  };
}