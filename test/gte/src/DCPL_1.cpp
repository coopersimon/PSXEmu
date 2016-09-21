#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, DCPL);

      tester.storeData(gte::RGB, 0x00000010);   // red = 16
      tester.storeData(gte::IR0, 0x00004000);   // ir0 = 4
      tester.storeData(gte::IR1, 0x00003000);   // ir1,2,3 = 3
      tester.storeData(gte::IR2, 0x00003000);
      tester.storeData(gte::IR3, 0x00003000);

      tester.storeControl(gte::RFC, 0x00000800);// far red = 128
      tester.storeControl(gte::GFC, 0x00000800);// far green = 128
      tester.storeControl(gte::BFC, 0x00000800);// far blue = 128

      /*  Mac 1 = (16 * 3) + 4*(B1[128 - (16 * 3)])
       *        =  48      + 4*(80)
       *        =  368
       *
       *  Mac 2, 3 = (0 * 3) + 4*(128 - 0)
       *        = 512
       *
       *  IR1,2,3 = Mac1,2,3
       *
       *  RGB2 = 0x00FFFFFF
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (368 << 4) &&
          tester.getData(gte::MAC2) == (512 << 4) &&
          tester.getData(gte::MAC3) == (512 << 4) &&
          tester.getData(gte::IR1) == (368 << 4) &&
          tester.getData(gte::IR2) == (512 << 4) &&
          tester.getData(gte::IR3) == (512 << 4) &&
          tester.getData(gte::RGB2) == 0x00FFFFFF)
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
