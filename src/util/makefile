CXX := clang++
CXXFLAGS := -O3 -fPIC

AR := ar

OBJECTS := HashMap.o
LIBRARY := libuetli_util.a


$(LIBRARY): $(OBJECTS)
	$(AR) rcs $@ $<

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)



