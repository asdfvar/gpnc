CC = g++

INC = -I.. -I../..
LIBDIR = -L../
LIBS = -lfft -lm

OBJS = test.o

all: $(OBJS)
	cd .. && $(MAKE)
	$(CC) $^ $(LIBDIR) $(LIBS) -o test

clean:
	cd .. && $(MAKE) clean
	rm -fv $(OBJS) test

%.o: %.cpp
	$(CC) $(INC) $^ -c
