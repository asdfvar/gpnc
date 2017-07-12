CC = mpic++
CL = ar rcs
FLAGS = -O3 -Wall
INC = -I/usr/local/include
LIBS = -pthread

OBJ = com.o

LIBEXE = libcom.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

com.o: com.cpp
	$(CC) $^ -c $(INC) $(LIBS)
