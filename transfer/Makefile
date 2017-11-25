CC    = mpic++
CL    = ar rcs
FLAGS = -O3 -Wall
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
