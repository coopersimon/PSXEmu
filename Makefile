CC=g++
CFLAGS=-std=c++11 -Iinclude

all : test

# test building the cpu
test : test2.cpp
	$(CC) $(CFLAGS) test2.cpp src/CPU.cpp src/Memory.cpp src/SCC.cpp -o testout
