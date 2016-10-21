#include "TestCPU.h"
#include "Memory.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(8);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(1, 2147483640);
      tester.storeReg(2, 100);
      tester.storeReg(3, 33);
      tester.constructInstruction(0, 1, 2, 3, 0, ADD);
      // exception is thrown. the exception handler simply places CAUSE into reg10
      tester.setMemLocation(0x80);
      tester.constructInstruction(COP0, 0, scc::CAUSE, 10, 0, 0);

      tester.run();

      if (tester.getReg(3) == 33 &&
          tester.getReg(10) == 12 << 2) // 12 == overflow
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "Reg 3: " << tester.getReg(3) << std::endl;
            std::cout << "Reg 10: " << tester.getReg(10) << std::endl;
      }

      delete mem;
}
