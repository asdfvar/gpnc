CC      = mpic++

FLAGS   = -g -Wall -ulimit

INC     = -I../middleware/com/    \
          -I../middleware/fio/    \
          -I../middleware/memory/ \
          -I../transfer           \
          -I../pm/                \
          -I../ioeng/             \
          -I../transfer/          \
          -I/usr/local/include/   \
#

LIBPATH = -L../middleware/fio/    \
          -L../middleware/memory/ \
          -L../transfer/          \
#

LIBS    = -lfio -lmem -ltrans -pthread

MIDDLEWARE = ../middleware/

masterOBJ = master.o       \
            master_task.o  \
            master_comm.o  \
#

slaveOBJ  = slave.o        \
            slave_task.o   \
            slave_comm.o   \
#

OBJ       = $(masterOBJ) $(slaveOBJ)

masterEXE = master
slaveEXE  = slave
EXE       = $(masterEXE) $(slaveEXE)

all: $(OBJ) 
	cd $(MIDDLEWARE)/fio    && $(MAKE)
	cd $(MIDDLEWARE)/memory && $(MAKE)
	cd ../transfer && $(MAKE)
	cd ../ioeng && $(MAKE)
	$(CC) $(masterOBJ) $(INC) $(FLAGS) $(LIBS) $(LIBPATH) -o $(masterEXE)
	$(CC) $(slaveOBJ) $(INC) $(FLAGS) $(LIBS) $(LIBPATH) -o $(slaveEXE)

clean:
	make -C $(MIDDLEWARE)/fio    clean
	make -C $(MIDDLEWARE)/memory clean
	make -C ../ioeng clean
	make -C ../transfer clean
	rm $(OBJ) $(EXE)

master.o: master.cpp
	$(CC) $^ -c $(INC) $(LIBS)

master_task.o: master_task.cpp
	$(CC) $^ -c $(INC) $(LIBS)

master_comm.o: master_comm.cpp
	$(CC) $^ -c $(INC) $(LIBS)

slave_comm.o: slave_comm.cpp
	$(CC) $^ -c $(INC) $(LIBS)

slave.o: slave.cpp
	$(CC) $^ -c $(INC) $(LIBS)

slave_task.o: slave_task.cpp
	$(CC) $^ -c $(INC) $(LIBS)
