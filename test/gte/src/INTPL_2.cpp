#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, INTPL);

      tester.storeData(gte::IR0, 0x00003A58);   // ir0 = 3.646484375
      tester.storeData(gte::IR1, 0x000027C0);   // ir1,2,3 = 2.484375
      tester.storeData(gte::IR2, 0x000027C0);
      tester.storeData(gte::IR3, 0x000027C0);

      tester.storeControl(gte::RFC, 0x000000A0);// far red = 10
      tester.storeControl(gte::GFC, 0x000000A0);// far green = 10
      tester.storeControl(gte::BFC, 0x00000040);// far blue = 4

      /*  Mac 1,2 = 2.484375 + 3.646484375*(B1[10 - 2.484375])
       *            ... 7.515625 (0x7840) -> (0x78) = 7.5
       *            ... (0x1b5940) = 27.3486328125
       *            = 29.8330078125 -> (0x1DD) = 29.8125
       *  
       *  Mac 3 = 2.484375 + 3.646484375 * (B1[4 - 2.484375])
       *          ... 1.515625 (0x1840) -> (0x18) = 1.5
       *          ... 5.4697265625
       *          = 7.9541015625 (0x7F44) -> (0x7F) = 7.9375
       *
       *  IR1,2,3 = Mac1,2,3
       *
       *  RGB2 = 0x00071D1D
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (0x1DD) &&
          tester.getData(gte::MAC2) == (0x1DD) &&
          tester.getData(gte::MAC3) == (0x7F) &&
          tester.getData(gte::IR1) == (0x1DD) &&
          tester.getData(gte::IR2) == (0x1DD) &&
          tester.getData(gte::IR3) == (0x7F) &&
          tester.getData(gte::RGB2) == 0x00071D1D &&
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
