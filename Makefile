CC=g++
CFLAGS=-std=c++11 -Iinclude

all : test

# test building the cpu
test : test.cpp
	$(CC) $(CFLAGS) test.cpp src/CPU.cpp src/Memory.cpp -o testout
