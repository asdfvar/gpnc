CC      = mpic++
INC     = -I../middleware/com/ -I../middleware/fio/ -I/usr/local/include
LIBPATH = -L../middleware/com/ -L../middleware/fio/
LIBS    = -lcom -lfio -pthread

MIDDLEWARE = ../middleware/

OBJ = main.o

EXE = main

all: $(OBJ)
	cd $(MIDDLEWARE)/com && $(MAKE)
	cd $(MIDDLEWARE)/fio && $(MAKE)
	$(CC) $^ $(INC) $(LIBS) $(LIBPATH) -o $(EXE)

clean:
	make -C $(MIDDLEWARE)/com clean
	make -C $(MIDDLEWARE)/fio clean
	rm $(OBJ) $(EXE)

main.o: main.cpp
	$(CC) $^ -c $(INC) $(LIBS)
