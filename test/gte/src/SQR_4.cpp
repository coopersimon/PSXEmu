#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(0, 0, 0, 0, 0, SQR);

      tester.storeData(gte::IR1, 0x00000FFF);   // ir1 = 4095
      tester.storeData(gte::IR2, 0x0000AF35);   // ir2 = -20683
      tester.storeData(gte::IR3, 0x0000FC19);   // ir3 = -999

      tester.run();

      if (tester.getData(gte::MAC1) == 0xFFE001 &&
          tester.getData(gte::MAC2) == 0x197F80F9 &&
          tester.getData(gte::MAC3) == 0xF3A71 &&
          tester.getData(gte::IR1) == 0x7FFF && // saturated
          tester.getData(gte::IR2) == 0x7FFF && // saturated
          tester.getData(gte::IR3) == 0x7FFF && // saturated
          tester.getControl(gte::FLAG) == 0x81C00000)
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
