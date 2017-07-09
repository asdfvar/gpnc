CC      = g++
OBJ     = driver.o
INC     = -I../middleware/com/ -I../middleware/fio/
LIBPATH = -L../middleware/com/ -L../middleware/fio/
LIBS    = -lcom -lfio

EXE = ioexe

all: $(OBJ)
	$(CC) $^ $(INC) $(LIBS) $(LIBPATH) -o $(EXE)

clean:
	rm $(OBJ) $(EXE)

driver.o: driver.cpp
	$(CC) $(INC) $^ -c
