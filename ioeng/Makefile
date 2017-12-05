baseDir = ..

# load defines
include $(baseDir)/defines.mk

FLAGS   = -g -Wall

OBJ     = data_extraction.o    \
          master_dex.o         \
          slave_dex.o          \
#

INC     = -I$(baseDir)/middleware/com/  \
          -I$(baseDir)/middleware/fio/  \
          -I$(baseDir)/middleware/mem/  \
          -I$(baseDir)/comm/            \
          -I$(PROC_INC)                 \
#

LIBPATH = -L$(baseDir)/middleware/fio/  \
          -L$(baseDir)/middleware/mem/  \
          -L$(baseDir)/comm/            \
#

LIBS    = -lfio -lcomm -lmem

EXE = ioexe

all: $(OBJ)
	$(MPI_CPP) $^ -o $(EXE) $(INC) $(FLAGS) $(LIBPATH) $(LIBS)

clean:
	rm $(OBJ) $(EXE)

data_extraction.o: data_extraction.cpp
	$(MPI_CPP) $(INC) $(LIBS) $(LIBPATH) $(LIBS) $^ -c

master_dex.o: master_dex.cpp
	$(MPI_CPP) $(INC) $(LIBS) $(LIBPATH) $(LIBS) $^ -c

slave_dex.o: slave_dex.cpp
	$(MPI_CPP) $(INC) $(LIBS) $(LIBPATH) $(LIBS) $^ -c
