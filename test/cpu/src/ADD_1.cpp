#include "TestCPU.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(8);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(1, 15);
      tester.storeReg(2, 25);
      tester.constructInstruction(0, 1, 2, 3, 0, ADD);
      
      tester.run();

      if (tester.getReg(3) == 40)
            std::cout << argv[0] << " Pass" << std::endl;
      else
            std::cout << argv[0] << " Fail" << std::endl;

      delete mem;
}
