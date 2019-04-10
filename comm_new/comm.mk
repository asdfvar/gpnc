include ../defines.mk

FLAGS = -Wall

OBJS = comm.o   \
       comm2d.o \
       comm3d.o \
#

LIB = libcomm.a

INC  = -I../middleware/com/ \
       $(PROC_INC)          \
#

all: $(OBJS)
	$(CL) $(LIB) $^

clean:
	rm -v $(OBJS) $(LIB)

%.o: %.cpp
	$(CPP) $(INC) $(FLAGS) -c $^
