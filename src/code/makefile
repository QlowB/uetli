CXX := clang++
CXXFLAGS := -O3 -fPIC -g

AR := ar

OBJECTS := StackMachine.o StackCodeGenerator.o AssemblyGenerator.o Assemblyx86_64.o
LIBRARY := libuetli_code.a


$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)



