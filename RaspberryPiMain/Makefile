# Set compiler to use
CC=g++
CFLAGS=-O3 `pkg-config opencv --cflags`
LDFLAGS=-O3 `pkg-config opencv --libs`
DEPS=tcpnetwork.h pose.h tracking.h udpnetwork.h config.h
OBJS=tcpnetwork.o pose.o tracking.o udpnetwork.o main.o

all:: main

main: $(OBJS)
	$(CC) -o $@ $^ -std=c++11 $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.stackdump

