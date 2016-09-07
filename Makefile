CC=g++
CFLAGS=-std=c++11 -Iinclude -Wall

all : test1

# test building the cpu
test1 : test/cpu/Test1.cpp
	$(CC) $(CFLAGS) test/cpu/Test1.cpp test/cpu/TestCPU.cpp src/CPU.cpp src/Memory.cpp src/SCC.cpp -o test1
