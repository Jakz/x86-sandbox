//
//  Compiler.cpp
//  JJXScriptEngine
//
//  Created by Jack on 9/6/15.
//  Copyright (c) 2015 Jack. All rights reserved.
//

#include "assembler.h"

#include "x86/opcodes.h"

#include <iostream>
#include <sstream>

using namespace assembler;
using namespace std;

Assembler::Assembler() : mode(Mode::BITS16) { }

void Assembler::error(const location& l, const std::string& m)
{
  cerr << "Error at " << file << ": " << (l.begin.line+1) << "," << l.begin.column << " : " << m << endl;
}

void Assembler::warning(const location& l, const std::string& m)
{
  cerr << "Warning at " << file << ": " << (l.begin.line+1) << "," << l.begin.column << " : " << m << endl;
}

void Assembler::error(const std::string& m)
{
  cerr << "Error: " << m << endl;
}

void Assembler::save(const std::string &text)
{
  FILE* out = fopen(text.c_str(), "wb");
  fwrite(&code[0], sizeof(decltype(code)::value_type), code.size(), out);
  fclose(out);
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

void Assembler::printCode()
{
  bool first = true;
  for (int i = 0; i < code.size(); ++i)
  {
    printf("%02X ",code[i]);
    if (i%8 == 0)
    {
      if (!first)
        printf("\n");
      first = false;
    }
  }
  
  if (code.size() % 8 != 0)
    printf("\n");
}

void Assembler::assemble(const std::string& text)
{
  return;
}

void Assembler::emitOperandPrefix(Mode operand)
{
  if ((operand == Mode::BITS16 && mode == Mode::BITS32) ||
      (operand == Mode::BITS32 && mode == Mode::BITS16) )
    emit(OP_PREFIX_OPERAND);
}

void Assembler::emitAddressPrefix(Mode address)
{
  if ((address == Mode::BITS16 && mode == Mode::BITS32) ||
      (address == Mode::BITS32 && mode == Mode::BITS16) )
    emit(OP_PREFIX_ADDRESS);
}

void Assembler::emitModePrefix(Mode operand, Mode address)
{
  emitOperandPrefix(operand);
  emitAddressPrefix(address);
}