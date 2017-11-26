CC = mpic++
CL = ar rcs
FLAGS = -O3 -Wall -g
INC = -I/usr/local/include
LIBS = -pthread

OBJ = memory.o

LIBEXE = libmem.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

memory.o: memory.cpp
	$(CC) $^ -c $(INC) $(LIBS)
