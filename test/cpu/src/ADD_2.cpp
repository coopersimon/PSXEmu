#include "TestCPU.h"
#include "Memory.h"
#include <iostream>

int main(int argc, char** argv)
{
      using namespace test;

      memoryInterface *mem = new RAMImpl(8);
      cpu device(mem);
      CPUtestbench tester(&device);

      tester.storeReg(10, 24367999);
      tester.storeReg(20, 11734850);
      tester.constructInstruction(0, 10, 20, 30, 0, ADD);
      
      tester.run();

      if (tester.getReg(30) == 36102849)
            std::cout << argv[0] << " Pass" << std::endl;
      else
            std::cout << argv[0] << " Fail" << std::endl;

      delete mem;
}
