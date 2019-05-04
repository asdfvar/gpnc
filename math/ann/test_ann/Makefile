CC = g++

OBJ = test_ann.o

.PHONY: all clean

INC    = -I..
LIBDIR = -L..
LIBS   = -lann

EXE = test_ann

all: $(OBJ)
	cd .. && $(MAKE) -f ann.mk
	$(CC) $(OBJ) $(LIBDIR) $(LIBS) -o $(EXE)

clean:
	make -C .. -f ann.mk clean
	rm -v $(EXE) $(OBJ)

%.o: %.cpp
	$(CC) $^ -c $(INC)
