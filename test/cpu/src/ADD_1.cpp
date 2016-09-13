#include "TestCPU.h"
#include "Memory.h"
#include <iostream>

int main()
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(8);
      cpu device(mem);
      CPUtestbench tester(&device);

      // r1 = 15
      tester.constructInstruction(ADDI, 0, 1, 15);
      // r2 = 25
      tester.constructInstruction(ADDI, 0, 2, 25);
      // r3 = r1 + r2
      tester.constructInstruction(0, 1, 2, 3, 0, ADD);
      
      tester.run();

      if (tester.getReg(3) == 40)
            std::cout << "Test 1 Pass" << std::endl;

      delete mem;
}
