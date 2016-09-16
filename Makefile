# MACROS
CXX=g++
CXXFLAGS=-std=c++11 -Iinclude -Wall
CPULINK=target/Memory.o target/Coprocessor.o target/SCC.o target/FixedPointMaths.o target/GTE.o target/CPU.o
TESTCPULINK=test/cpu/src/TestCPU.o target/Memory.o target/Coprocessor.o target/SCC.o target/FixedPointMaths.o target/GTE.o target/CPU.o
TESTGTELINK=test/gte/src/TestGTE.o target/Coprocessor.o target/FixedPointMaths.o target/GTE.o

# MAIN BUILDS
testall : testcpu testgte

testcpu : test/cpu/bin/ADD_1 test/cpu/bin/ADD_2 test/cpu/bin/ADD_3 test/cpu/bin/ADD_4

testgte : test/gte/bin/DCPL_1

# TEST CPU
test/cpu/src/TestCPU.o : test/cpu/src/TestCPU.cpp test/cpu/src/TestCPU.h
	$(CXX) $(CXXFLAGS) -c test/cpu/src/TestCPU.cpp -o $@

test/cpu/bin/ADD_1 : test/cpu/src/ADD_1.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) $? -o $@

test/cpu/bin/ADD_2 : test/cpu/src/ADD_2.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) $? -o $@

test/cpu/bin/ADD_3 : test/cpu/src/ADD_3.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) $? -o $@

test/cpu/bin/ADD_4 : test/cpu/src/ADD_4.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) $? -o $@


# TEST GTE
test/gte/src/TestGTE.o : test/gte/src/TestGTE.cpp test/gte/src/TestGTE.h
	$(CXX) $(CXXFLAGS) -c test/gte/src/TestGTE.cpp -o $@

test/gte/bin/DCPL_1 : test/gte/src/DCPL_1.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) $? -o $@

# OBJECT FILES
target/Memory.o : src/Memory.cpp include/Memory.h include/PSException.h
	$(CXX) $(CXXFLAGS) -c src/Memory.cpp -o $@

target/Coprocessor.o : src/Coprocessor.cpp include/Coprocessor.h include/PSException.h
	$(CXX) $(CXXFLAGS) -c src/Coprocessor.cpp -o $@

target/SCC.o : src/SCC.cpp include/SCC.h include/PSException.h
	$(CXX) $(CXXFLAGS) -c src/SCC.cpp -o $@

target/FixedPointMaths.o : src/FixedPointMaths.cpp include/FixedPointMaths.h
	$(CXX) $(CXXFLAGS) -c src/FixedPointMaths.cpp -o $@

target/GTE.o : src/GTE.cpp include/GTE.h
	$(CXX) $(CXXFLAGS) -c src/GTE.cpp -o $@

target/CPU.o : src/CPU.cpp include/CPU.h
	$(CXX) $(CXXFLAGS) -c src/CPU.cpp -o $@


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
	rm test/gte/src/*.o
	rm test/gte/bin/*
