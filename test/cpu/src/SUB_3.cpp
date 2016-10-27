#include "TestCPU.h"
#include "Memory.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(8);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(5, -12345);
      tester.storeReg(15, -54321);
      tester.constructInstruction(0, 5, 15, 20, 0, SUB);
      
      tester.run();

      if (tester.getReg(20) == 41976)
            std::cout << argv[0] << " Pass" << std::endl;
      else
            std::cout << argv[0] << " Fail" << std::endl;

      delete mem;
}
