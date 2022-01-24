AR = ar rcs

OBJS = fft.o

LIB = libfft.a

all: $(OBJS)
	$(AR) $(LIB) $^

clean:
	rm -fv $(OBJS) $(LIB)

%.o: %.cpp
	$(CC) -I../ $^ -c
