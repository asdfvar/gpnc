include ../../defines.mk

OBJS_1 = module_0.o
OBJS_2 = module_1.o

LIBDIRS = -L../
LIBS    = -lcomm

INC = -I../         \
      -I/$(MPI_INC) \
#

COMM_LIB:
	cd ../ && $(MAKE) -f comm.mk

MODULE_0: $(OBJS_1)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_0.exe

MODULE_1: $(OBJS_2)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_1.exe

all: COMM_LIB MODULE_0 MODULE_1

clean:
	cd ../ && $(MAKE) -f comm.mk clean
	rm -v $(OBJS_1) module_0.exe
	rm -v $(OBJS_2) module_1.exe

%.o: %.cpp
	$(MPI_CPP) $(INC) $(LIBDIRS) $(LIBS) -c $^
