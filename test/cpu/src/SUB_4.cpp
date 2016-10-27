#include "TestCPU.h"
#include "Memory.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(8);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(1, 0x80000000);
      tester.storeReg(2, 10);
      tester.storeReg(3, 277);
      tester.constructInstruction(0, 1, 2, 3, 0, SUB);
      tester.setMemLocation(0x80);
      tester.constructInstruction(COP0, 0, scc::CAUSE, 5, 0, 0);
      
      tester.run();

      if (tester.getReg(3) == 277 &&
          tester.getReg(5) == 12 << 2) // 12 == overflow
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "Reg 3: " << tester.getReg(3) << std::endl;
            std::cout << "Reg 5: " << tester.getReg(5) << std::endl;
      }

      delete mem;
}
