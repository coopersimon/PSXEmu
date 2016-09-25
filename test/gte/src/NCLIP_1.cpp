#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, NCLIP);

      tester.storeData(gte::SXY0, 0x0005000A);  // sx0 = 10, sy0 = 5
      tester.storeData(gte::SXY1, 0x0019000F);  // sx1 = 15, sy1 = 25
      tester.storeData(gte::SXY2, 0x000A000A);  // sx2 = 10, sy2 = 10


      /*  Mac 0 = 10 * 25 + 15 * 10 + 10 * 5 - 10 * 10 - 15 * 5 - 10 * 25
       *        = 250 + 150 + 50 - 100 - 75 - 250
       *        = 25
       */

      tester.run();

      if (tester.getData(gte::MAC0) == 25 &&
          tester.getControl(gte::FLAG) == 0x0)
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "MAC0: " << tester.getData(gte::MAC0) << std::endl;
            std::cout << "FLAG: " << std::hex << tester.getControl(gte::FLAG) << std::endl;
      }
}
