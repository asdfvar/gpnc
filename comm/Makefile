baseDir = ..

# load defines
include $(baseDir)/defines.mk

FLAGS = -Wall -g
LIBS  = -pthread

INC   = -I/usr/local/include         \
        -I$(baseDir)/middleware/com/ \
        $(PROC_INC)                  \
#

OBJ   = extract_data.o \
        master_comm.o  \
        slave_comm.o   \
#

LIBEXE = libcomm.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

extract_data.o: extract_data.cpp
	$(CPP) $^ -c $(INC) $(LIBS)

master_comm.o: master_comm.cpp
	$(CPP) $^ -c $(INC) $(LIBS)

slave_comm.o: slave_comm.cpp
	$(CPP) $^ -c $(INC) $(LIBS)
