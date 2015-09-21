CXX := clang++
CXXFLAGS := -O3
LINKFLAGS := -Lparser/ -luetli_parser -Lsemantic -luetli_semantic -Lutil -luetli_util -Lcode -luetli_code

OBJECTS := main.o
LIBRARIES := parser/libuetli_parser.a semantic/libuetli_semantic.a util/libuetli_util.a code/libuetli_code.a
EXECUTABLE := uetli



$(EXECUTABLE): $(OBJECTS) $(LIBRARIES)
	$(CXX) $(OBJECTS) $(LINKFLAGS) -o $@


%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: parser/libuetli_parser.a
parser/libuetli_parser.a:
	cd parser && $(MAKE)

.PHONY: semantic/libuetli_semantic.a
semantic/libuetli_semantic.a:
	cd semantic && $(MAKE)

.PHONY: util/libuetli_code.a
code/libuetli_code.a:
	cd code && $(MAKE)

.PHONY: util/libuetli_util.a
util/libuetli_util.a:
	cd util && $(MAKE)

.PHONY: clear
clear:
	rm *.o *.a parser/*.o parser/*.a semantic/*.o semantic/*.a code/*.o code/*.a util/*.o uetli/*.a


