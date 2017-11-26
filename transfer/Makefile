#CC      = mpic++
CC      = /home/euler/user_libs/openmpi_3p0p0/bin/mpic++
CL    = ar rcs
FLAGS = -O3 -Wall -g
LIBS  = -pthread

INC   = -I/usr/local/include \
        -I../middleware/com/ \
        -I../pm/             \
#

OBJ   = extract_data.o

LIBEXE = libtrans.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

extract_data.o: extract_data.cpp
	$(CC) $^ -c $(INC) $(LIBS)
