#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, 0, 0, 0, 0, SQR);

      tester.storeData(gte::IR1, 0x000000A8);   // ir1 = 168
      tester.storeData(gte::IR2, 0x000007A1);   // ir2 = 1953
      tester.storeData(gte::IR3, 0x00002C36);   // ir3 = 11318

      tester.run();

      if (tester.getData(gte::MAC1) == 0x6E40 &&
          tester.getData(gte::MAC2) == 0x3A3341 &&
          tester.getData(gte::MAC3) == 0x7A29B64 &&
          tester.getData(gte::IR1) == 0x6E40 &&
          tester.getData(gte::IR2) == 0x7FFF && // saturated
          tester.getData(gte::IR3) == 0x7FFF && // saturated
          tester.getControl(gte::FLAG) == 0x80C00000)
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
