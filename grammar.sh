flex -c++ --outfile=src/assembler/x86lexer.cpp src/assembler/x86lexer.l
bison -v --output-file=src/assembler/x86parser.cpp src/assembler/x86parser.ypp