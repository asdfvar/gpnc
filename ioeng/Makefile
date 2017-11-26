baseDir = ..

# load defines
include $(baseDir)/defines.mk

FLAGS   = -g -Wall

OBJ     = data_extraction.o    \
          master_dex.o         \
          slave_dex.o          \
#

INC     = -I../middleware/com/    \
          -I../middleware/fio/    \
          -I../middleware/memory/ \
          -I../pm/                \
          -I../transfer/          \
          -I$(PROC_INC)                   \
#

LIBPATH = -L../middleware/fio/    \
          -L../middleware/memory/ \
          -L../transfer/          \
#

LIBS    = -lfio -ltrans -lmem

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
