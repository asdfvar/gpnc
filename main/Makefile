baseDir = ..

# load defines
include $(baseDir)/defines.mk

FLAGS   = -g -Wall -ulimit

INC     = -I$(baseDir)/middleware/com/    \
          -I$(baseDir)/middleware/fio/    \
          -I$(baseDir)/middleware/memory/ \
          -I$(baseDir)/comm/              \
          -I$(baseDir)/ioeng/             \
          -I$(PROC_INC)                   \
#

LIBPATH = -L$(baseDir)/middleware/fio/    \
          -L$(baseDir)/middleware/memory/ \
          -L$(baseDir)/comm/              \
#

LIBS    = -lfio -lmem -lcomm -pthread

MIDDLEWARE = $(baseDir)/middleware/

masterOBJ = master.o       \
            master_task.o  \
#

slaveOBJ  = slave.o        \
            slave_task.o   \
#

OBJ       = $(masterOBJ) $(slaveOBJ)

masterEXE = master
slaveEXE  = slave
EXE       = $(masterEXE) $(slaveEXE)

all: $(OBJ) 
	cd $(MIDDLEWARE)/fio    && $(MAKE)
	cd $(MIDDLEWARE)/memory && $(MAKE)
	cd $(baseDir)/comm && $(MAKE)
	cd $(baseDir)/ioeng && $(MAKE)
	$(MPI_CPP) $(masterOBJ) $(INC) $(FLAGS) $(LIBS) $(LIBPATH) -o $(masterEXE)
	$(MPI_CPP) $(slaveOBJ) $(INC) $(FLAGS) $(LIBS) $(LIBPATH) -o $(slaveEXE)

clean:
	make -C $(MIDDLEWARE)/fio    clean
	make -C $(MIDDLEWARE)/memory clean
	make -C $(baseDir)/ioeng clean
	make -C $(baseDir)/comm clean
	rm $(OBJ) $(EXE)

# identify all object files from *.cpp files
%.o: %.cpp
	$(MPI_CPP) $^ -c $(INC) $(LIBS)
