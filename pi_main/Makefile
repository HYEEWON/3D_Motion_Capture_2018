# Set compiler to use
CC=g++
CFLAGS=-O3 `pkg-config opencv --cflags`
LDFLAGS=-O3 `pkg-config opencv --libs`
DEPS=net.h pos.h tracking.h rasnet.h config.h
OBJS=net.o pos.o mainpi2.o tracking.o rasnet.o

all:: pi3

pi3: $(OBJS)
	$(CC) -o $@ $^ -std=c++11 $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.stackdump

