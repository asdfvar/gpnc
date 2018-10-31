include ../defines.mk

FLAGS = -Wall

OBJS = comm.o

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
