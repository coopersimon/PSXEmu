#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, DPCS);

      tester.storeData(gte::RGB, 0x00000010);   // red = 16

      tester.storeData(gte::IR0, 0x00003800);   // ir0 = 3.5

      tester.storeControl(gte::RFC, 0x00000200);// far red = 32
      tester.storeControl(gte::GFC, 0x00000100);// far green = 16
      tester.storeControl(gte::BFC, 0x000000A0);// far blue = 10

      /*  Mac 1 = 16 + 3.5*B1(32 - 16)
       *        =  16      + 3.5 * 16
       *        =  16 + 56 = 72
       *
       *  Mac 2 = 0  + 3.5*B1(16)
       *        = 56
       *
       *  Mac 3 = 0  + 3.5*B1(10)
       *        = 35
       *
       *  IR1,2,3 = Mac1,2,3
       *
       *  RGB2 = 0x00233848
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (72 << 4) &&
          tester.getData(gte::MAC2) == (56 << 4) &&
          tester.getData(gte::MAC3) == (35 << 4) &&
          tester.getData(gte::IR1) == (72 << 4) &&
          tester.getData(gte::IR2) == (56 << 4) &&
          tester.getData(gte::IR3) == (35 << 4) &&
          tester.getData(gte::RGB2) == 0x00233848)
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "MAC1: " << tester.getData(gte::MAC1) << std::endl;
            std::cout << "MAC2: " << tester.getData(gte::MAC2) << std::endl;
            std::cout << "MAC3: " << tester.getData(gte::MAC3) << std::endl;
            std::cout << "IR1: " << tester.getData(gte::IR1) << std::endl;
            std::cout << "IR2: " << tester.getData(gte::IR2) << std::endl;
            std::cout << "IR3: " << tester.getData(gte::IR3) << std::endl;
            std::cout << "RGB2: " << std::hex << tester.getData(gte::RGB2) << std::endl;
      }
}
