CXX := g++
CXXFLAGS := -g
LINKFLAGS :=

YACC := bison
YACCFLAGS := -d
LEX := flex
LEXFLAGS :=

OBJECTS := $(patsubst %.cpp, %.o, $(wildcard *.cpp */*.cpp)) \
	parser/Parser.o parser/Lexer.o
LIBRARIES := 
EXECUTABLE := uetli


all: $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ $(LINKFLAGS) -o $@


%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.cpp: %.y
	$(YACC) -o $@ $< $(YACCFLAGS)

%.cpp: %.l
	$(LEX) -o $@ $< $(LEXFLAGS)



clear:
	rm -f *.o *.a parser/*.o parser/*.a semantic/*.o semantic/*.a code/*.o code/*.a util/*.o uetli/*.a assembly/*.o assembly/*.a


