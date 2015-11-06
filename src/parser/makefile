CXX := g++
CXXFLAGS := -fPIC -g

AR := ar

YACC := bison
YACCFLAGS := -d

LEX := flex
LEXFLAGS :=

OBJECTS := Lexer.o Parser.o ParseObject.o Identifier.o uetli_parser.o 
LIBRARY := libuetli_parser.a


$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)
#	clang++ *.o -o prog


Parser.cpp: Parser.y Lexer.cpp
	$(YACC) -o $@ $< $(YACCFLAGS)

Lexer.cpp: Lexer.l
	$(LEX) -o $@ $< $(LEXFLAGS)
#	$(LEX) -o $@ --header-file=Lexer.h $< $(LEXFLAGS)

%.o: %.cpp Parser.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

