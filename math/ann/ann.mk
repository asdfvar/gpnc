basedir = ../..

# load defines
include $(basedir)/defines.mk

LIBAR = libann.a

FLAGS = -O3

LIBS = -lm

OBJ = ann.o layer.o

all: $(OBJ)
	$(CL) $(LIBAR) $^

clean:
	rm $(OBJ) $(LIBAR)

%.o: %.cpp
	$(CPP) $^ -c $(LIBS) $(FLAGS)
