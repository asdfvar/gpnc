CC      = mpic++

OBJ     = data_extraction.o    \
          master_dex.o         \
          slave_dex.o          \
#

INC     = -I../middleware/com/ \
          -I../middleware/fio/ \
          -I../pm/             \
          -I../transfer/       \
#

LIBPATH = -L../middleware/fio/ \
          -L../transfer/       \

LIBS    = -lfio -ltrans

EXE = ioexe

all: $(OBJ)
	$(CC) $^ -o $(EXE) $(INC) $(LIBPATH) $(LIBS)

clean:
	rm $(OBJ) $(EXE)

data_extraction.o: data_extraction.cpp
	$(CC) $(INC) $(LIBS) $(LIBPATH) $(LIBS) $^ -c

master_dex.o: master_dex.cpp
	$(CC) $(INC) $(LIBS) $(LIBPATH) $(LIBS) $^ -c

slave_dex.o: slave_dex.cpp
	$(CC) $(INC) $(LIBS) $(LIBPATH) $(LIBS) $^ -c
