CXX := clang++
CXXFLAGS := -O3

OBJECTS := main.o
EXECUTABLE := uetli


$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@


%.o: %.c
		$(CXX) -c -o $@ $< $(CXXFLAGS)



