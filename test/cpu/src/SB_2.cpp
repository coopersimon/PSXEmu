#include "TestCPU.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(10);
      cpu device(mem);
      CPUtestbench tester(&device);

      mem->writeWord(0x100, 0x12345678);
      tester.storeReg(1, 0xFF);
      tester.constructInstruction(SB, 0, 1, 0x101);

      tester.run();

      if (mem->readWord(0x100) == 0x1234FF78)
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "Mem 0x100 = " << mem->readWord(0x100) << std::endl;
      }

      delete mem;
}
