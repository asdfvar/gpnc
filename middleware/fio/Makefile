baseDir = ../..

# load defines
include $(baseDir)/defines.mk

OBJ = fio.o string_utils.o

LIBEXE = libfio.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

fio.o: fio.cpp
	$(CPP) $^ -c $(INC)

string_utils.o: string_utils.cpp
	$(CPP) $^ -c $(INC)
