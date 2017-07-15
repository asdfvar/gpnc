CC = g++
CL = ar rcs
FLAGS = -O3 -Wall

OBJ = fio.o string_utils.o

LIBEXE = libfio.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

fio.o: fio.cpp
	$(CC) $^ -c $(INC)

string_utils.o: string_utils.cpp
	$(CC) $^ -c $(INC)
