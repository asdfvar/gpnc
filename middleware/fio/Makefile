baseDir = ../..

# load defines
include $(baseDir)/defines.mk

OBJ = fio.o string_utils.o

LIBAR = libfio.a

all: $(OBJ)
	$(CL) $(LIBAR) $^

clean:
	rm $(OBJ) $(LIBAR)

fio.o: fio.cpp
	$(CPP) $^ -c $(INC)

string_utils.o: string_utils.cpp
	$(CPP) $^ -c $(INC)
