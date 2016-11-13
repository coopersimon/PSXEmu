#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, NCS);

      tester.storeControl(gte::L11L12, 0x20001000); // light matrix
      tester.storeControl(gte::L13L21, 0x50003000); //  1  2  3
      tester.storeControl(gte::L22L23, 0x30004000); //  5  4  3
      tester.storeControl(gte::L31L32, 0x2000F000); // -1  2 -3
      tester.storeControl(gte::L33, 0xD000);

      tester.storeData(gte::VXY0, 0x20002000); // V0: 2 2 2
      tester.storeData(gte::VZ0, 0x2000);

      tester.storeControl(gte::LR1LR2, 0x20001000); // colour matrix
      tester.storeControl(gte::LR3LG1, 0x50003000); //  1  2  3
      tester.storeControl(gte::LG2LG3, 0x50005000); //  5  5  5
      tester.storeControl(gte::LB1LB2, 0x1000E000); // -2  1 -2
      tester.storeControl(gte::LB3, 0xE000);

      tester.storeControl(gte::RBK, 0x00005000); // red background: 5
      tester.storeControl(gte::GBK, 0x00008000); // green background: 8
      tester.storeControl(gte::BBK, 0x00006000); // blue background: 6

      /*  Mid 1 = 1*2 + 2*2 + 3*2 = 12
       *  Mid 2 = 5*2 + 4*2 + 3*2 = 24
       *  Mid 3 = -1*2 + 2*2 + -3*2 = -4
       *
       *  (fractions below shown in hex)
       *  Mac 1 = 5 + 1*7.FFF + 2*7.FFF + 3*-4 = 5 + 7.FFF + 15.FFE -12 = 16.FFD->10.F
       *  Mac 2 = 8 + 5*7.FFF + 5*7.FFF + 5*-4 = 8 + 39.FFA + 39.FFA -20 = 67.FF9->43.F
       *  Mac 3 = 6 + -2*7.FFF + 1*7.FFF -2*-4 = 6 - 15.FFE + 7.FFF + 8 = 14 - 7.FFF = 6.001->6.0
       *
       *  IR1,2,3 = Mac1,2,3
       *  RGB = 0x063B10
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (0x10F) &&
          tester.getData(gte::MAC2) == (0x43F) &&
          tester.getData(gte::MAC3) == (0x60) &&
          tester.getData(gte::IR1) == (0x10F) &&
          tester.getData(gte::IR2) == (0x43F) &&
          tester.getData(gte::IR3) == (0x60) &&
          tester.getData(gte::RGB2) == (0x64310) &&
          tester.getControl(gte::FLAG) == 0x81800000)
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
