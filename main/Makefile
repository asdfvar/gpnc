CC      = mpic++

INC     = -I../middleware/com/    \
          -I../middleware/fio/    \
          -I../middleware/memory/ \
          -I../parameters/        \
          -I../pm/                \
          -I../ioeng/             \
          -I/usr/local/include/   \
#

LIBPATH = -L../middleware/fio/    \
          -L../middleware/memory/ \
#

LIBS    = -lfio -lmem -pthread

MIDDLEWARE = ../middleware/

masterOBJ = master.o      \
            master_comm.o \
#

slaveOBJ  = slave.o       \
            worker_task.o \
#

OBJ       = $(masterOBJ) $(slaveOBJ)

masterEXE = master
slaveEXE  = slave
EXE       = $(masterEXE) $(slaveEXE)

all: $(OBJ) 
	cd $(MIDDLEWARE)/fio    && $(MAKE)
	cd $(MIDDLEWARE)/memory && $(MAKE)
	cd ../ioeng && $(MAKE)
	$(CC) $(masterOBJ) $(INC) $(LIBS) $(LIBPATH) -o $(masterEXE)
	$(CC) $(slaveOBJ) $(INC) $(LIBS) $(LIBPATH) -o $(slaveEXE)

clean:
	make -C $(MIDDLEWARE)/fio    clean
	make -C $(MIDDLEWARE)/memory clean
	make -C ../ioeng clean
	rm $(OBJ) $(EXE)

master.o: master.cpp
	$(CC) $^ -c $(INC) $(LIBS)

master_comm.o: master_comm.cpp
	$(CC) $^ -c $(INC) $(LIBS)

slave.o: slave.cpp
	$(CC) $^ -c $(INC) $(LIBS)

worker_task.o: worker_task.cpp
	$(CC) $^ -c $(INC) $(LIBS)
