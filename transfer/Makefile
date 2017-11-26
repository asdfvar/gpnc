baseDir = ..

# load defines
include $(baseDir)/defines.mk

FLAGS = -Wall -g
LIBS  = -pthread

INC   = -I/usr/local/include         \
        -I$(baseDir)/middleware/com/ \
        -I$(baseDir)/pm/             \
        $(PROC_INC)                  \
#

OBJ   = extract_data.o

LIBEXE = libtrans.a

all: $(OBJ)
	$(CL) $(LIBEXE) $^

clean:
	rm $(OBJ) $(LIBEXE)

extract_data.o: extract_data.cpp
	$(CPP) $^ -c $(INC) $(LIBS)
