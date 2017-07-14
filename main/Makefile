CC      = mpic++
INC     = -I../middleware/com/ -I../middleware/fio/ -I../parameters/ -I/usr/local/include
LIBPATH = -L../middleware/com/ -L../middleware/fio/
LIBS    = -lcom -lfio -pthread

MIDDLEWARE = ../middleware/

masterOBJ = master.o
slaveOBJ  = slave.o
OBJ       = $(masterOBJ) $(slaveOBJ)

masterEXE = master
slaveEXE  = slave
EXE       = $(masterEXE) $(slaveEXE)

all: $(OBJ) 
	cd $(MIDDLEWARE)/com && $(MAKE)
	cd $(MIDDLEWARE)/fio && $(MAKE)
	$(CC) $(masterOBJ) $(INC) $(LIBS) $(LIBPATH) -o $(masterEXE)
	$(CC) $(slaveOBJ) $(INC) $(LIBS) $(LIBPATH) -o $(slaveEXE)

clean:
	make -C $(MIDDLEWARE)/com clean
	make -C $(MIDDLEWARE)/fio clean
	rm $(OBJ) $(EXE)

master.o: master.cpp
	$(CC) $^ -c $(INC) $(LIBS)

slave.o: slave.cpp
	$(CC) $^ -c $(INC) $(LIBS)
