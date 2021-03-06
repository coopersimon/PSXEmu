#include "TestCPU.h"
#include "Memory.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(8);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(1, 177);
      tester.constructInstruction(COP2, 4, 1, gte::VXY0, 0, 0);

      tester.constructInstruction(COP2, 0, gte::VXY0, 2, 0, 0);

      tester.run();

      if (tester.getReg(2) == 177)
            std::cout << argv[0] << " Pass" << std::endl;
      else
            std::cout << argv[0] << " Fail" << std::endl;

      delete mem;
}
