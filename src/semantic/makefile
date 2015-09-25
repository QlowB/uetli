CXX := clang++
CXXFLAGS := -O3 -fPIC -g

AR := ar

OBJECTS := AttributedSyntaxTree.o TreeBuilder.o Scope.o
LIBRARY := libuetli_semantic.a


$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)



