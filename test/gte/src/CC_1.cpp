#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, CC);

      tester.storeControl(gte::LR1LR2, 0x20001000); // colour matrix
      tester.storeControl(gte::LR3LG1, 0x50003000); //  1  2  3
      tester.storeControl(gte::LG2LG3, 0x50005000); //  5  5  5
      tester.storeControl(gte::LB1LB2, 0x1000E000); // -2  1 -2
      tester.storeControl(gte::LB3, 0xE000);

      tester.storeData(gte::IR1, 0x3000); // IR1: 3
      tester.storeData(gte::IR2, 0x2000); // IR2: 2
      tester.storeData(gte::IR3, 0x1000); // IR3: 1

      tester.storeControl(gte::RBK, 0x00008000); // red background: 8
      tester.storeControl(gte::GBK, 0x0000C000); // green background: 12
      tester.storeControl(gte::BBK, 0x00010000); // blue background: 16
      
      tester.storeData(gte::RGB, 0x00070503); // R G B: 3,5,7

      /*  Mac 1 = 8 + 1 * 3 + 2 * 2 + 3 * 1    = 18
       *  Mac 2 = 12 + 5 * 3 + 5 * 2 + 5 * 1   = 42
       *  Mac 3 = 16 + -2 * 3 + 1 * 2 + -2 * 1 = 10
       *
       *  IR1,2,3 = 7FFF
       *
       *  Mac 1 = 7FFF * 3 = 17F(FD)
       *  Mac 2 = 7FFF * 5 = 27F(FB)
       *  Mac 3 = 7FFF * 7 = 37F(F9)
       *
       *  IR1,2,3 = Mac1,2,3
       *  RGB = 0x372717
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (0x17F) &&
          tester.getData(gte::MAC2) == (0x27F) &&
          tester.getData(gte::MAC3) == (0x37F) &&
          tester.getData(gte::IR1) == (0x17F) &&
          tester.getData(gte::IR2) == (0x27F) &&
          tester.getData(gte::IR3) == (0x37F) &&
          tester.getData(gte::RGB2) == (0x372717) &&
          tester.getControl(gte::FLAG) == 0x81C00000)
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "MAC1: " << std::hex << tester.getData(gte::MAC1) << std::endl;
            std::cout << "MAC2: " << std::hex << tester.getData(gte::MAC2) << std::endl;
            std::cout << "MAC3: " << std::hex << tester.getData(gte::MAC3) << std::endl;
            std::cout << "IR1: " << std::hex << tester.getData(gte::IR1) << std::endl;
            std::cout << "IR2: " << std::hex << tester.getData(gte::IR2) << std::endl;
            std::cout << "IR3: " << std::hex << tester.getData(gte::IR3) << std::endl;
            std::cout << "RGB2: " << std::hex << tester.getData(gte::RGB2) << std::endl;
            std::cout << "FLAG: " << std::hex << tester.getControl(gte::FLAG) << std::endl;
      }
}
