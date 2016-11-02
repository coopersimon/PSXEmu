#include "TestCPU.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(10);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(1, 654321);
      tester.constructInstruction(SW, 0, 1, 0x108);

      tester.run();

      if (mem->readWord(0x108) == 654321)
            std::cout << argv[0] << " Pass" << std::endl;
      else
            std::cout << argv[0] << " Fail" << std::endl;

      delete mem;
}
