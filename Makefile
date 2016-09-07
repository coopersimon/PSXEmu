CC=g++
CFLAGS=-std=c++11 -Iinclude

all : test

# test building the cpu
test3 : test3.cpp
	$(CC) $(CFLAGS) test3.cpp src/CPU.cpp src/Memory.cpp src/SCC.cpp -o testout

test2 : test2.cpp
	$(CC) $(CFLAGS) test2.cpp src/CPU.cpp src/Memory.cpp src/SCC.cpp -o testout
