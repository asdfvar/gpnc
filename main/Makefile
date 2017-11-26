baseDir = ..

# load defines
include $(baseDir)/defines.mk

FLAGS   = -g -Wall -ulimit

INC     = -I$(baseDir)/middleware/com/    \
          -I$(baseDir)/middleware/fio/    \
          -I$(baseDir)/middleware/memory/ \
          -I$(baseDir)/transfer           \
          -I$(baseDir)/pm/                \
          -I$(baseDir)/ioeng/             \
          -I$(baseDir)/transfer/          \
          -I$(PROC_INC)                   \
#

LIBPATH = -L$(baseDir)/middleware/fio/    \
          -L$(baseDir)/middleware/memory/ \
          -L$(baseDir)/transfer/          \
#

LIBS    = -lfio -lmem -ltrans -pthread

MIDDLEWARE = $(baseDir)/middleware/

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
	cd $(baseDir)/transfer && $(MAKE)
	cd $(baseDir)/ioeng && $(MAKE)
	$(MPI_CPP) $(masterOBJ) $(INC) $(FLAGS) $(LIBS) $(LIBPATH) -o $(masterEXE)
	$(MPI_CPP) $(slaveOBJ) $(INC) $(FLAGS) $(LIBS) $(LIBPATH) -o $(slaveEXE)

clean:
	make -C $(MIDDLEWARE)/fio    clean
	make -C $(MIDDLEWARE)/memory clean
	make -C $(baseDir)/ioeng clean
	make -C $(baseDir)/transfer clean
	rm $(OBJ) $(EXE)

master.o: master.cpp
	$(MPI_CPP) $^ -c $(INC) $(LIBS)

master_task.o: master_task.cpp
	$(MPI_CPP) $^ -c $(INC) $(LIBS)

master_comm.o: master_comm.cpp
	$(MPI_CPP) $^ -c $(INC) $(LIBS)

slave_comm.o: slave_comm.cpp
	$(MPI_CPP) $^ -c $(INC) $(LIBS)

slave.o: slave.cpp
	$(MPI_CPP) $^ -c $(INC) $(LIBS)

slave_task.o: slave_task.cpp
	$(MPI_CPP) $^ -c $(INC) $(LIBS)
