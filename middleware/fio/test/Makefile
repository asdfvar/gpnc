CC      = g++
LIBPATH = -L..
INC     = -I..
LIBS    = -lfio

OBJ = test.o

all: $(OBJ)
	cd .. && $(MAKE)
	$(CC) $^ $(INC) $(LIBS) $(LIBPATH) -o test

clean:
	make -C .. clean
	rm $(OBJ) test

test.o: test.cpp
	$(CC) $(INC) $^ -c
