#include "TestCPU.h"
#include "Memory.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(8);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(1, 550);
      tester.storeReg(2, -235);
      tester.constructInstruction(0, 1, 2, 3, 0, ADD);
      
      tester.run();

      if (tester.getReg(3) == 315)
            std::cout << argv[0] << " Pass" << std::endl;
      else
            std::cout << argv[0] << " Fail" << std::endl;

      delete mem;
}
