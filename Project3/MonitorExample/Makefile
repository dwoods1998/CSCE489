CC=g++
CXXFLAGS=-g -Wall -Wextra -O0 -pthread
DEPS = Item.h Monitor.h Bounded_Buffer.h
OBJ = babyyoda.o Item.o Monitor.o Bounded_Buffer.o
BIN = babyyoda 

%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o *~ $(BIN)
