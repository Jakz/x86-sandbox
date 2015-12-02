%{
	#include "x86parser.hpp"
  #include "x86lexer.h"
  #include "assembler.h"
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

