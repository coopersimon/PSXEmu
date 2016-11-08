#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, 0, 0, 3, 1, MVMVA);

      tester.storeControl(gte::R11R12, 0x28001800); // rotation matrix
      tester.storeControl(gte::R13R21, 0x20003800); //  1.5  2.5  3.5
      tester.storeControl(gte::R22R23, 0x40003000); //  2    3    4
      tester.storeControl(gte::R31R32, 0x34005000); //  5    3.25 1
      tester.storeControl(gte::R33, 0x1000);

      tester.storeData(gte::VXY0, 0x20001000); // V0: 1 2 3
      tester.storeData(gte::VZ0, 0x3000);

      /*tester.storeControl(gte::TRX, 0); // no extra addition
      tester.storeControl(gte::TRY, 0);
      tester.storeControl(gte::TRZ, 0);*/


      /*  Mac 1 = 1.5 * 1 + 2.5 * 2 + 3.5 * 3   = 17
       *  Mac 2 = 2 * 1 + 3 * 2 + 4 * 3 = 20
       *  Mac 3 = 5 * 1 + 3.25 * 2 + 1 * 3  = 14.5
       *
       *  IR1,2,3 = Mac1,2,3
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (17 << 4) &&
          tester.getData(gte::MAC2) == (20 << 4) &&
          tester.getData(gte::MAC3) == ((14 << 4) + (1 << 3)) &&
          tester.getData(gte::IR1) == (17 << 4) &&
          tester.getData(gte::IR2) == (20 << 4) &&
          tester.getData(gte::IR3) == ((14 << 4) + (1 << 3)) &&
          tester.getControl(gte::FLAG) == 0x00000000)
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
