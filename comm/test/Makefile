include ../../defines.mk

OBJS_0 = module_0.o
OBJS_1 = module_1.o
OBJS_2 = module_2.o
OBJS_3 = module_3.o
OBJS_4 = module_4.o

LIBDIRS = -L../
LIBS    = -lcomm

INC = -I../         \
      -I/$(MPI_INC) \
#

all: COMM_LIB MODULE_0 MODULE_1 MODULE_2 MODULE_3 MODULE_4

COMM_LIB:
	cd ../ && $(MAKE) -f comm.mk

MODULE_0: $(OBJS_0)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_0.exe

MODULE_1: $(OBJS_1)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_1.exe

MODULE_2: $(OBJS_2)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_2.exe

MODULE_3: $(OBJS_3)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_3.exe

MODULE_4: $(OBJS_4)
	$(MPI_CPP) $^ $(INC) $(LIBDIRS) $(LIBS) -o module_4.exe

clean:
	cd ../ && $(MAKE) -f comm.mk clean
	rm -v $(OBJS_0) module_0.exe
	rm -v $(OBJS_1) module_1.exe
	rm -v $(OBJS_2) module_2.exe
	rm -v $(OBJS_3) module_3.exe
	rm -v $(OBJS_4) module_4.exe

%.o: %.cpp
	$(MPI_CPP) $(INC) $(LIBDIRS) $(LIBS) -c $^
