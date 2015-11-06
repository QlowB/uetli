CXX := g++
CXXFLAGS := -g -fPIC

AR := ar

OBJECTS := HashMap.o
LIBRARY := libuetli_util.a


$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $<

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)



