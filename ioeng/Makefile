CC      = mpic++
OBJ     = driver.o
INC     = -I../middleware/com/ -I../middleware/fio/
LIBPATH = -L../middleware/fio/
LIBS    = -lfio

EXE = ioexe

all: $(OBJ)
	$(CC) $^ -o $(EXE) $(INC) $(LIBPATH) $(LIBS)

clean:
	rm $(OBJ) $(EXE)

driver.o: driver.cpp
	$(CC) $(INC) $(LIBS) $(LIBPATH) $(LIBS) $^ -c
