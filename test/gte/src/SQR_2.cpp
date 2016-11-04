#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(1, 0, 0, 0, 0, SQR);

      tester.storeData(gte::IR1, 0x000019FB);   // ir1 = 1.623779296875
      tester.storeData(gte::IR2, 0x000007A1);   // ir2 = 0.95361328125
      tester.storeData(gte::IR3, 0x0000D65C);   // ir3 = -2.6

      tester.run();

      if (tester.getData(gte::MAC1) == 0x2A2F &&
          tester.getData(gte::MAC2) == 0x03A3 &&
          tester.getData(gte::MAC3) == 0x6C5F &&
          tester.getData(gte::IR1) == 0x2A2F &&
          tester.getData(gte::IR2) == 0x03A3 &&
          tester.getData(gte::IR3) == 0x6C5F &&
          tester.getControl(gte::FLAG) == 0x00000000)
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
            std::cout << "FLAG: " << std::hex << tester.getControl(gte::FLAG) << std::endl;
      }
}
