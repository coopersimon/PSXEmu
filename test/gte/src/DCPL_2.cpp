#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, DCPL);

      tester.storeData(gte::RGB, 0x0000000C);   // red = 12
      tester.storeData(gte::IR0, 0x00002A00);   // ir0 = 2.625
      tester.storeData(gte::IR1, 0x000035C0);   // ir1,2,3 = 3.359375
      tester.storeData(gte::IR2, 0x000035C0);
      tester.storeData(gte::IR3, 0x000035C0);

      tester.storeControl(gte::RFC, 0x00000500);// far red = 80
      tester.storeControl(gte::GFC, 0x00000500);// far green = 80
      tester.storeControl(gte::BFC, 0x00000500);// far blue = 80

      /*  Mac 1 = (12 * 3.359375) + 2.625*(B1[80 - (12 * 3.359375)])
       *        =  40.3125        + 2.625*(39.6875)
       *        =                   104.1796875->104.125
       *        =  144.4375
       *
       *  Mac 2, 3 = (0 * 3.4) + 2.625*(80 - 0)
       *        = 210
       *
       *  IR1,2,3 = Mac1,2,3
       *
       *  RGB2 = 0x00D2D290
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (144.4375 * 16) &&
          tester.getData(gte::MAC2) == (210 * 16) &&
          tester.getData(gte::MAC3) == (210 * 16) &&
          tester.getData(gte::IR1) == (144.4375 * 16) &&
          tester.getData(gte::IR2) == (210 * 16) &&
          tester.getData(gte::IR3) == (210 * 16) &&
          tester.getData(gte::RGB2) == 0x00D2D290)
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
