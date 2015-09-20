CXX := clang++
CXXFLAGS := -O3 -fPIC

AR := ar

OBJECTS := AttributedSyntaxTree.o TreeBuilder.o
LIBRARY := libuetli_semantic.a


$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)



