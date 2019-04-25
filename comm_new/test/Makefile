include ../../defines.mk

OBJS_1 = module_1.o
OBJS_2 = module_2.o

LIBDIRS = -L../
LIBS    = -lcomm

INC = -I../         \
      -I/$(MPI_INC) \
#

COMM_LIB:
	cd ../ && $(MAKE) -f comm.mk

MODULE_1: $(OBJS_1)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_1.exe

MODULE_2: $(OBJS_2)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_2.exe

all: COMM_LIB MODULE_1 MODULE_2

clean:
	cd ../ && $(MAKE) -f comm.mk clean
	rm -v $(OBJS_1) module_1.exe
	rm -v $(OBJS_2) module_2.exe

%.o: %.cpp
	$(MPI_CPP) $(INC) $(LIBDIRS) $(LIBS) -c $^
