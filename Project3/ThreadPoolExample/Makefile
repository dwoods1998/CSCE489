CC=g++
CXXFLAGS=-g -Wall -Wextra -O0 -pthread
DEPS = Item.h BoundedBuffer.h ThreadPool.h Monitor.h
OBJ = babyyoda.o Item.o BoundedBuffer.o ThreadPool.o Monitor.o
BIN = babyyoda 

%.o: %.cpp $(DEPS)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o *~ $(BIN)
