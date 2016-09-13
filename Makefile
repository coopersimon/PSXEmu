# MACROS
CC=g++
CFLAGS=-std=c++11 -Iinclude -Wall

# MAIN BUILDS
testcpu : test/cpu/bin/ADD_1


# TESTS
test/cpu/src/TestCPU.o : test/cpu/src/TestCPU.cpp test/cpu/src/TestCPU.h
	$(CC) $(CFLAGS) -c test/cpu/src/TestCPU.cpp -o test/cpu/src/TestCPU.o

test/cpu/bin/ADD_1 : test/cpu/src/ADD_1.cpp test/cpu/src/TestCPU.o target/Memory.o target/Coprocessor.o target/SCC.o target/FixedPointMaths.o target/GTE.o target/CPU.o
	$(CC) $(CFLAGS) test/cpu/src/ADD_1.cpp test/cpu/src/TestCPU.o target/Memory.o target/Coprocessor.o target/SCC.o target/FixedPointMaths.o target/GTE.o target/CPU.o -o test/cpu/bin/ADD_1

	

#test1 : test/cpu/Test1.cpp
#	$(CC) $(CFLAGS) test/cpu/Test1.cpp test/cpu/TestCPU.cpp src/CPU.cpp src/Memory.cpp src/SCC.cpp src/GTE.cpp src/FixedPointMaths.cpp src/Coprocessor.cpp -o test1

# OBJECT FILES
target/Memory.o : src/Memory.cpp include/Memory.h include/PSException.h
	$(CC) $(CFLAGS) -c src/Memory.cpp -o target/Memory.o

target/Coprocessor.o : src/Coprocessor.cpp include/Coprocessor.h include/PSException.h
	$(CC) $(CFLAGS) -c src/Coprocessor.cpp -o target/Coprocessor.o

target/SCC.o : src/SCC.cpp include/SCC.h include/PSException.h
	$(CC) $(CFLAGS) -c src/SCC.cpp -o target/SCC.o

target/FixedPointMaths.o : src/FixedPointMaths.cpp include/FixedPointMaths.h
	$(CC) $(CFLAGS) -c src/FixedPointMaths.cpp -o target/FixedPointMaths.o

target/GTE.o : src/GTE.cpp include/GTE.h
	$(CC) $(CFLAGS) -c src/GTE.cpp -o target/GTE.o

target/CPU.o : src/CPU.cpp include/CPU.h
	$(CC) $(CFLAGS) -c src/CPU.cpp -o target/CPU.o


# HEADER DEPENDENCIES
include/Register.h : include/Memory.h

include/FixedPointMaths.h : include/Memory.h

include/Coprocessor.h : include/Memory.h include/Register.h

include/SCC.h : include/Coprocessor.h

include/GTE.h : include/Coprocessor.h include/FixedPointMaths.h

include/CPU.h : include/SCC.h include/GTE.h

# CLEAN
clean :
	rm target/*
	rm test/cpu/src/*.o
	rm test/cpu/bin/*
