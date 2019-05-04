include ../defines.mk

FLAGS = -Wall

OBJS = comm.o   \
       comm2d.o \
       comm3d.o \
#

LIB = libcomm.a

INC  = -I../middleware/com/ \
       -I/$(MPI_INC)        \
#

all: $(OBJS)
	$(CL) $(LIB) $^

clean:
	rm -v $(OBJS) $(LIB)

%.o: %.cpp
	$(MPI_CPP) $(INC) $(FLAGS) -c $^
