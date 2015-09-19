CXX := clang++
CXXFLAGS := -O3
LINKFLAGS := -Lparser/ -luetli_parser -Lsemantic -luetli_semantic -static

OBJECTS := main.o parser/libuetli_parser.a semantic/libuetli_semantic.a
EXECUTABLE := uetli



$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LINKFLAGS) -o $@


%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: parser/libuetli_parser.a
parser/libuetli_parser.a:
	cd parser && $(MAKE)

.PHONY: semantic/libuetli_semantic.a
semantic/libuetli_semantic.a:
	cd semantic && $(MAKE)



