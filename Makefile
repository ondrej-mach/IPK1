CXX=g++
CXXFLAGS=-std=c++17 -pedantic -Wall -Wextra -g
EXECUTABLE=hinfosvc
OBJECTS=main.o server.o info.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *.so *.a $(XLOGIN).zip $(EXECUTABLE)
