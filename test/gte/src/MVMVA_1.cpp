#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, 0, 0, 3, 1, MVMVA);

      tester.storeControl(gte::R11R12, 0x40003000); // rotation matrix
      tester.storeControl(gte::R13R21, 0x30005000); //  3  4  5
      tester.storeControl(gte::R22R23, 0x10002000); //  3  2  1
      tester.storeControl(gte::R31R32, 0x5000B000); // -5  5 -5
      tester.storeControl(gte::R33, 0xB000);

      tester.storeData(gte::VXY0, 0x30002000); // V0: 2 3 4
      tester.storeData(gte::VZ0, 0x4000);

      /*tester.storeControl(gte::TRX, 0); // no extra addition
      tester.storeControl(gte::TRY, 0);
      tester.storeControl(gte::TRZ, 0);*/


      /*  Mac 1 = 3 * 2 + 4 * 3 + 5 * 4   = 38
       *  Mac 2 = 3 * 2 + 2 * 3 + 1 * 4   = 16
       *  Mac 3 = -5 * 2 + 5 * 3 + -5 * 4 = -15
       *
       *  IR1,2,3 = Mac1,2,3
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (38 << 12) &&
          tester.getData(gte::MAC2) == (16 << 12) &&
          tester.getData(gte::MAC3) == (unsigned(-15) << 12) &&
          tester.getData(gte::IR1) == (0x7FFF) &&
          tester.getData(gte::IR2) == (0x7FFF) &&
          tester.getData(gte::IR3) == (0x0000) &&
          tester.getControl(gte::FLAG) == 0x81c00000)
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
