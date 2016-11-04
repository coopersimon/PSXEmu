# MACROS
CXX=clang++
CXXFLAGS=-std=c++11 -Iinclude -Wall -g
CPULINK=target/Memory.o target/Coprocessor.o target/SCC.o target/FixedPointMaths.o target/GTE.o target/CPU.o
TESTCPULINK=test/cpu/src/TestCPU.o target/Memory.o target/Coprocessor.o target/SCC.o target/FixedPointMaths.o target/GTE.o target/CPU.o
TESTGTELINK=test/gte/src/TestGTE.o target/Coprocessor.o target/FixedPointMaths.o target/GTE.o

# MAIN BUILDS
testall : testcpu testgte

testcpu : \
test/cpu/bin/ADD_1 test/cpu/bin/ADD_2 test/cpu/bin/ADD_3 test/cpu/bin/ADD_4 test/cpu/bin/ADD_5 test/cpu/bin/ADD_6 \
test/cpu/bin/SUB_1 test/cpu/bin/SUB_2 test/cpu/bin/SUB_3 test/cpu/bin/SUB_4 test/cpu/bin/SUB_5 \
test/cpu/bin/SB_1 test/cpu/bin/SB_2 test/cpu/bin/SB_3 test/cpu/bin/SB_4 \
test/cpu/bin/SW_1 test/cpu/bin/SW_2 test/cpu/bin/SW_3 \
test/cpu/bin/MFCz_1 \
test/cpu/bin/MTCz_1

testgte : \
test/gte/bin/DCPL_1 test/gte/bin/DCPL_2 test/gte/bin/DCPL_3 \
test/gte/bin/SQR_1 test/gte/bin/SQR_2 test/gte/bin/SQR_3 test/gte/bin/SQR_4 \
test/gte/bin/INTPL_1 test/gte/bin/INTPL_2 \
test/gte/bin/NCLIP_1 \
test/gte/bin/MVMVA_1 test/gte/bin/MVMVA_2 test/gte/bin/MVMVA_3 \
test/gte/bin/CC_1


# TEST CPU
test/cpu/src/TestCPU.o : test/cpu/src/TestCPU.cpp test/cpu/src/TestCPU.h
	$(CXX) $(CXXFLAGS) -c test/cpu/src/TestCPU.cpp -o $@

test/cpu/bin/ADD_1 : test/cpu/src/ADD_1.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/ADD_1.cpp $(TESTCPULINK) -o $@

test/cpu/bin/ADD_2 : test/cpu/src/ADD_2.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/ADD_2.cpp $(TESTCPULINK) -o $@

test/cpu/bin/ADD_3 : test/cpu/src/ADD_3.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/ADD_3.cpp $(TESTCPULINK) -o $@

test/cpu/bin/ADD_4 : test/cpu/src/ADD_4.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/ADD_4.cpp $(TESTCPULINK) -o $@

test/cpu/bin/ADD_5 : test/cpu/src/ADD_5.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/ADD_5.cpp $(TESTCPULINK) -o $@

test/cpu/bin/ADD_6 : test/cpu/src/ADD_6.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/ADD_6.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SUB_1 : test/cpu/src/SUB_1.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SUB_1.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SUB_2 : test/cpu/src/SUB_2.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SUB_2.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SUB_3 : test/cpu/src/SUB_3.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SUB_3.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SUB_4 : test/cpu/src/SUB_4.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SUB_4.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SUB_5 : test/cpu/src/SUB_5.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SUB_5.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SB_1 : test/cpu/src/SB_1.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SB_1.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SB_2 : test/cpu/src/SB_2.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SB_2.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SB_3 : test/cpu/src/SB_3.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SB_3.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SB_4 : test/cpu/src/SB_4.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SB_4.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SW_1 : test/cpu/src/SW_1.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SW_1.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SW_2 : test/cpu/src/SW_2.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SW_2.cpp $(TESTCPULINK) -o $@

test/cpu/bin/SW_3 : test/cpu/src/SW_3.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/SW_3.cpp $(TESTCPULINK) -o $@

test/cpu/bin/MFCz_1 : test/cpu/src/MFCz_1.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/MFCz_1.cpp $(TESTCPULINK) -o $@

test/cpu/bin/MTCz_1 : test/cpu/src/MTCz_1.cpp $(TESTCPULINK)
	$(CXX) $(CXXFLAGS) test/cpu/src/MTCz_1.cpp $(TESTCPULINK) -o $@


# TEST GTE
test/gte/src/TestGTE.o : test/gte/src/TestGTE.cpp test/gte/src/TestGTE.h
	$(CXX) $(CXXFLAGS) -c test/gte/src/TestGTE.cpp -o $@

test/gte/bin/DCPL_1 : test/gte/src/DCPL_1.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/DCPL_1.cpp $(TESTGTELINK) -o $@

test/gte/bin/DCPL_2 : test/gte/src/DCPL_2.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/DCPL_2.cpp $(TESTGTELINK) -o $@

test/gte/bin/DCPL_3 : test/gte/src/DCPL_3.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/DCPL_3.cpp $(TESTGTELINK) -o $@

test/gte/bin/SQR_1 : test/gte/src/SQR_1.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/SQR_1.cpp $(TESTGTELINK) -o $@

test/gte/bin/SQR_2 : test/gte/src/SQR_2.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/SQR_2.cpp $(TESTGTELINK) -o $@

test/gte/bin/SQR_3 : test/gte/src/SQR_3.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/SQR_3.cpp $(TESTGTELINK) -o $@

test/gte/bin/SQR_4 : test/gte/src/SQR_4.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/SQR_4.cpp $(TESTGTELINK) -o $@

test/gte/bin/INTPL_1 : test/gte/src/INTPL_1.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/INTPL_1.cpp $(TESTGTELINK) -o $@

test/gte/bin/INTPL_2 : test/gte/src/INTPL_2.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/INTPL_2.cpp $(TESTGTELINK) -o $@

test/gte/bin/NCLIP_1 : test/gte/src/NCLIP_1.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/NCLIP_1.cpp $(TESTGTELINK) -o $@

test/gte/bin/MVMVA_1 : test/gte/src/MVMVA_1.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/MVMVA_1.cpp $(TESTGTELINK) -o $@

test/gte/bin/MVMVA_2 : test/gte/src/MVMVA_2.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/MVMVA_2.cpp $(TESTGTELINK) -o $@

test/gte/bin/MVMVA_3 : test/gte/src/MVMVA_3.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/MVMVA_3.cpp $(TESTGTELINK) -o $@

test/gte/bin/CC_1 : test/gte/src/CC_1.cpp $(TESTGTELINK)
	$(CXX) $(CXXFLAGS) test/gte/src/CC_1.cpp $(TESTGTELINK) -o $@


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
clean : cleantarget cleancputest cleangtetest

cleantarget :
	rm target/*

cleancputest :
	rm test/cpu/src/*.o
	rm test/cpu/bin/*

cleangtetest :
	rm test/gte/src/*.o
	rm test/gte/bin/*
