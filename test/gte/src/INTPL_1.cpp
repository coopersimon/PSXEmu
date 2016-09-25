#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, INTPL);

      tester.storeData(gte::IR0, 0x00004000);   // ir0 = 4
      tester.storeData(gte::IR1, 0x00003000);   // ir1,2,3 = 3
      tester.storeData(gte::IR2, 0x00003000);
      tester.storeData(gte::IR3, 0x00003000);

      tester.storeControl(gte::RFC, 0x00000080);// far red = 8
      tester.storeControl(gte::GFC, 0x00000080);// far green = 8
      tester.storeControl(gte::BFC, 0x00000080);// far blue = 8

      /*  Mac 1,2,3 = 3 + 4*(B1[8 - 3])
       *            = 23
       *
       *
       *  IR1,2,3 = Mac1,2,3
       *
       *  RGB2 = 0x00171717
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (23 << 4) &&
          tester.getData(gte::MAC2) == (23 << 4) &&
          tester.getData(gte::MAC3) == (23 << 4) &&
          tester.getData(gte::IR1) == (23 << 4) &&
          tester.getData(gte::IR2) == (23 << 4) &&
          tester.getData(gte::IR3) == (23 << 4) &&
          tester.getData(gte::RGB2) == 0x00171717 &&
          tester.getControl(gte::FLAG) == 0x0)
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
            std::cout << "FLAG: " << std::hex << tester.getControl(gte::FLAG) << std::endl;
      }
}
