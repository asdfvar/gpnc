CC      = g++
LIBPATH = -L..
INC     = -I..
LIBS    = -lfio
FLAGS   = -g -Wall -O3

OBJ = test.o

all: $(OBJ)
	cd .. && $(MAKE)
	$(CC) $^ $(INC) $(FLAGS) $(LIBS) $(LIBPATH) -o test

clean:
	make -C .. clean
	rm $(OBJ) test

test.o: test.cpp
	$(CC) $(INC) $^ -c
