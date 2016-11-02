#include "TestCPU.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(10);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(1, 123456);
      mem->writeWord(16, 54321);
      tester.constructInstruction(SW, 0, 1, 15);
      // exception is thrown
      tester.setMemLocation(0x80);
      tester.constructInstruction(COP0, 0, scc::CAUSE, 10, 0, 0);

      tester.run();

      if (mem->readWord(16) == 54321 &&
          tester.getReg(10) == 5 << 2) // 5 == unaligned store
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "Mem 0x10: " << mem->readWord(16) << std::endl;
            std::cout << "Reg 10: " << tester.getReg(10) << std::endl;
      }

      delete mem;
}
