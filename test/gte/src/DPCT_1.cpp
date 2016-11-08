#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, DPCT);

      tester.storeData(gte::RGB, 0x10000000); // CODE = 0x10
      tester.storeData(gte::RGB2, 0x01000010);   // r0 = 16
      tester.storeData(gte::RGB2, 0x02001100);   // g1 = 17
      tester.storeData(gte::RGB2, 0x030A0008);   // r2 = 8, b2 = 10

      tester.storeData(gte::IR0, 0x00002000);   // ir0 = 2

      tester.storeControl(gte::RFC, 0x000001A0);// far red = 26
      tester.storeControl(gte::GFC, 0x00000400);// far green = 64
      tester.storeControl(gte::BFC, 0x00000130);// far blue = 19

      /*  Mac 1 = 16 + 2*B1(26 - 16)      Mac 1 = 0  + 2*B1(26)         Mac 1 = 8  + 2*B1(26 - 8)
       *        = 36                            = 52                          = 44
       *
       *  Mac 2 = 0  + 2*B2(64)           Mac 2 = 17 + 2*B2(64 - 17)    Mac 2 = 128
       *        = 128                           = 111
       *
       *  Mac 3 = 0  + 2*B3(19)           Mac 3 = 38                    Mac 3 = 10 + 2*B3(19 - 10)
       *        = 38                                                          = 28
       *
       *  IR1,2,3 = Mac1,2,3
       *
       *  RGB0 = 0x10268024               RGB1 = 0x10266F34             RGB2 = 0x101C802C
       *
       */

      tester.run();

      if (tester.getData(gte::MAC1) == (44 << 4) &&
          tester.getData(gte::MAC2) == (128 << 4) &&
          tester.getData(gte::MAC3) == (28 << 4) &&
          tester.getData(gte::IR1) == (44 << 4) &&
          tester.getData(gte::IR2) == (128 << 4) &&
          tester.getData(gte::IR3) == (28 << 4) &&
          tester.getData(gte::RGB0) == 0x10268024 &&
          tester.getData(gte::RGB1) == 0x10266F34 &&
          tester.getData(gte::RGB2) == 0x101C802C)
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
            std::cout << "RGB0: " << std::hex << tester.getData(gte::RGB0) << std::endl;
            std::cout << "RGB1: " << std::hex << tester.getData(gte::RGB1) << std::endl;
            std::cout << "RGB2: " << std::hex << tester.getData(gte::RGB2) << std::endl;
      }
}
