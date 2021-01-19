SRCS = main.cpp calibration.cpp
CC = g++

all:
	$(CC) -o main $(SRCS) `pkg-config --cflags --libs opencv`
clean: 
	rm -f *.exe

