#include "TestCPU.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(10);
      cpu device(mem);
      CPUtestbench tester(&device);

      mem->writeWord(0x110, 0xFEDCBA09);
      tester.storeReg(1, 0x55);
      tester.storeReg(2, 0x77);
      tester.storeReg(15, 0x100);
      tester.constructInstruction(SB, 15, 1, 0x10);
      tester.constructInstruction(SB, 15, 2, 0x12);

      tester.run();

      if (mem->readWord(0x110) == 0xFE77BA55)
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "Mem 0x110 = " << mem->readWord(0x110) << std::endl;
      }

      delete mem;
}
