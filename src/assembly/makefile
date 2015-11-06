CXX := g++
CXXFLAGS := -fPIC -g

AR := ar

OBJECTS := AssemblyGenerator.o Assemblyx86_64.o
LIBRARY := libuetli_assembly.a


$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)



