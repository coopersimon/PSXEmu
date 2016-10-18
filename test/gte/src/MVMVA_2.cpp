#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, 0, 0, 3, 1, MVMVA);

      tester.storeControl(gte::R11R12, 0x40001000); // rotation matrix
      tester.storeControl(gte::R13R21, 0xE0007000); //  1  4  7
      tester.storeControl(gte::R22R23, 0xE0002000); // -2  2 -2
      tester.storeControl(gte::R31R32, 0xD0002000); //  2 -3  2
      tester.storeControl(gte::R33, 0x2000);

      tester.storeData(gte::VXY0, 0x20001000); // V0: 1 2 3
      tester.storeData(gte::VZ0, 0x3000);

      /*tester.storeControl(gte::TRX, 0); // no extra addition
      tester.storeControl(gte::TRY, 0);
      tester.storeControl(gte::TRZ, 0);*/


      /*  Mac 1 = 1 * 1 + 4 * 2 + 7 * 3   = 30
       *  Mac 2 = -2 * 1 + 2 * 2 + -2 * 3 = -4
       *  Mac 3 = 2 * 1 + -3 * 2 + 2 * 3  = 2
       *
       *  IR1,2,3 = Mac1,2,3
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (30 << 12) &&
          tester.getData(gte::MAC2) == (-4 << 12) &&
          tester.getData(gte::MAC3) == (2 << 12) &&
          tester.getData(gte::IR1) == (0x7FFF) &&
          tester.getData(gte::IR2) == (0x0000) &&
          tester.getData(gte::IR3) == (0x2000) &&
          tester.getControl(gte::FLAG) == 0x81800000)
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
            std::cout << "FLAG: " << std::hex << tester.getControl(gte::FLAG) << std::endl;
      }
}
