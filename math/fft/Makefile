CC = g++

OBJS = fft.o

all: $(OBJS)
	$(CC) $^ -lm -o fft

clean:
	rm -fv $(OBJS) fft

%.o: %.cpp
	$(CC) -I../ $^ -c
