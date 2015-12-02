//
//  Compiler.cpp
//  JJXScriptEngine
//
//  Created by Jack on 9/6/15.
//  Copyright (c) 2015 Jack. All rights reserved.
//

#include "assembler.h"

#include <iostream>
#include <sstream>

using namespace assembler;
using namespace std;

Assembler::Assembler() { }

void Assembler::error(const location& l, const std::string& m)
{
  cerr << "Compiler error at " << file << ": " << l.begin.line << "," << l.begin.column << " : " << m << endl;
}

void Assembler::error(const std::string& m)
{
  cerr << "Compiler error: " << m << endl;
}

bool Assembler::parseString(const std::string &text)
{
  file = "none";
  
  bool shouldGenerateTrace = false;
  
  istringstream ss(text);
  
  assembler::Lexer lexer = assembler::Lexer(*this, &ss);
  assembler::Parser parser(lexer, *this);
  parser.set_debug_level(shouldGenerateTrace);
  return parser.parse() == 0;
}

void Assembler::assemble(const std::string& text)
{
  return;
}