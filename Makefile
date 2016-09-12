CC=g++
CFLAGS=-std=c++11 -Iinclude -Wall

# test building the cpu
test1 : test/cpu/Test1.cpp
	$(CC) $(CFLAGS) test/cpu/Test1.cpp test/cpu/TestCPU.cpp src/CPU.cpp src/Memory.cpp src/SCC.cpp src/GTE.cpp src/FixedPointMaths.cpp src/Coprocessor.cpp -o test1

#fptest : test/fixedPoint/Test1.cpp
#	$(CC) $(CFLAGS) test/fixedPoint/Test1.cpp src/FixedPointMaths.cpp -o testFP
