%{
	#include "x86parser.hpp"
  #include "x86lexer.h"
  #include "assembler.h"
  #include "x86/opcodes.h"
  #include <string>
  
  typedef assembler::Parser::token token;
  
  #define yyterminate() assembler::Parser::make_END(loc);


//
  
  static std::string buffer;
  
  static assembler::location loc;
  
  using namespace assembler;
%}

%x sstring
%x scomment

%option nodefault
%option noyywrap
%option c++
%option yyclass="Lexer"
%option prefix="x86"


%{
  # define YY_USER_ACTION  loc.columns((int)yyleng);
%}


%%

%{
  loc.step();
%}

"true" { return Parser::make_BOOL(true, loc); }
"false" { return Parser::make_BOOL(false, loc); }

":" { return Parser::make_COLON(loc); }
"," { return Parser::make_COMMA(loc); }
"'" { return Parser::make_QUOTE(loc); }
";" { return Parser::make_SEMICOL(loc); }
"?" { return Parser::make_QUESTION(loc); }

"=" { return Parser::make_ASSIGN(loc); }

"+" { return Parser::make_PLUS(loc); }
"-" { return Parser::make_MINUS(loc); }
"*" { return Parser::make_TIMES(loc); }
"/" { return Parser::make_DIV(loc); }
"%" { return Parser::make_MOD(loc); }
"&&" { return Parser::make_LAND(loc); }
"||" { return Parser::make_LOR(loc); }

"+=" { return Parser::make_ASSIGN_PLUS(loc); }
"-=" { return Parser::make_ASSIGN_MINUS(loc); }
"*=" { return Parser::make_ASSIGN_TIMES(loc); }
"/=" { return Parser::make_ASSIGN_DIV(loc); }
"%=" { return Parser::make_ASSIGN_MOD(loc); }
"&&=" { return Parser::make_ASSIGN_LAND(loc); }
"||=" { return Parser::make_ASSIGN_LOR(loc); }

"<" { return Parser::make_LESS(loc); }
"<=" { return Parser::make_LESSEQ(loc); }
">" { return Parser::make_GREATER(loc); }
">=" { return Parser::make_GREATEREQ(loc); }
"==" { return Parser::make_EQUAL(loc); }
"!=" { return Parser::make_NEQUAL(loc); }

"(" { return Parser::make_LPAREN(loc); }
")" { return Parser::make_RPAREN(loc); }
"[" { return Parser::make_LBRACK(loc); }
"]" { return Parser::make_RBRACK(loc); }
"{" { return Parser::make_LBRACE(loc); }
"}" { return Parser::make_RBRACE(loc); }


"++" { return Parser::make_INCR(loc); }
"--" { return Parser::make_DECR(loc); }

".." { return Parser::make_RANGE(loc); }
"->" { return Parser::make_FIELD(loc); }

"var" { return Parser::make_VAR(loc); }

"while" { return Parser::make_WHILE(loc); }
"if" { return Parser::make_IF(loc); }
"else" { return Parser::make_ELSE(loc); }
"elif" { return Parser::make_ELIF(loc); }
"for" { return Parser::make_FOR(loc); }

"void" { return Parser::make_VOID_TYPE(loc); }

"al"|"AL" { return Parser::make_REG8(AL, loc); }
"ah"|"AH" { return Parser::make_REG8(AH, loc); }
"ax"|"AX" { return Parser::make_REG16(AX, loc); }
"eax"|"EAX" { return Parser::make_REG32(EAX, loc); }

"bl"|"BL" { return Parser::make_REG8(BL, loc); }
"bh"|"BH" { return Parser::make_REG8(BH, loc); }
"bx"|"BX" { return Parser::make_REG16(BX, loc); }
"ebx"|"EBX" { return Parser::make_REG32(EBX, loc); }

"cl"|"CL" { return Parser::make_REG8(CL, loc); }
"ch"|"CH" { return Parser::make_REG8(CH, loc); }
"cx"|"CX" { return Parser::make_REG16(CX, loc); }
"ecx"|"ECX" { return Parser::make_REG32(ECX, loc); }

"dl"|"DL" { return Parser::make_REG8(DL, loc); }
"dh"|"DH" { return Parser::make_REG8(DH, loc); }
"dx"|"DX" { return Parser::make_REG16(DX, loc); }
"edx"|"EDX" { return Parser::make_REG32(EDX, loc); }



"AAA"|"aaa" { return Parser::make_OP8(OP_AAA, loc); }
"NOP"|"nop" { return Parser::make_OP8(OP_NOP, loc); }
"MOV"|"mov" { return Parser::make_MOV(loc); }


"0x"[a-fA-F0-9]+ { return Parser::make_U32(strtol( &yytext[2], NULL, 16), loc); }
[a-fA-F0-9]+"H" { return Parser::make_U32(strtol( yytext, NULL, 16), loc); }
[a-fA-F0-9]+"h" { return Parser::make_U32(strtol( yytext, NULL, 16), loc); }
"0b"[0-1]+ { return Parser::make_U32(strtol( &yytext[2], NULL, 2), loc); }
[0-1]+"B" { return Parser::make_U32(strtol( yytext, NULL, 2), loc); }
[0-1]+"b" { return Parser::make_U32(strtol( yytext, NULL, 2), loc); }
[-]?[0-9]+ { return Parser::make_U32(atoi(yytext), loc);}
[0-9]+\.[0-9]*"f"? { return Parser::make_FLOAT(atof(yytext), loc); }


<INITIAL>[\"'] { buffer.clear(); BEGIN(sstring); }

<sstring>[\"'] { BEGIN(INITIAL); return Parser::make_LITERAL(buffer.c_str(), loc); }
<sstring>\\n { buffer += '\n'; }
<sstring>\\0 { buffer += '\0'; printf(">>>>>>>> ANTANI!\n"); }
<sstring>\\(.|\n) { buffer += yytext[1]; }
<sstring>[^\\\n\"']+ { char *ptr = yytext; while (*ptr) buffer += *ptr++; }

<INITIAL>"/*" { BEGIN(scomment); }
<scomment>"*/" { BEGIN(INITIAL); }
<scomment>\n|"\r\n" { loc.lines(yyleng); loc.step();  }
<scomment>.+ { }


[a-zA-Z_][a-zA-Z0-9_]* { return Parser::make_IDENTIFIER(yytext,loc); }

"//"[^\n]*\n { Parser::make_EOL(loc); loc.step(); }


\n|"\r\n" { Parser::make_EOL(loc); loc.lines((int)yyleng); }
[ \t] { loc.step(); /* ignore whitespace */ }
. { assembler.error(loc, "Unrecognized character."); }

<<EOF>> { return yyterminate(); }

%%

//"'"[ -~]"'" { return Parser::make_U32(yytext[1], loc); }

// \"(\\.|[^\\"])*\" { j80lval.str = strdup(&yytext[1]); j80lval.str[strlen(yytext)-2] = 0; return STRING_LITERAL; }
// <sstring>\\[a-fA-F0-9]{1,2} { buffer += strtol(&yytext[1], NULL, 16); }

