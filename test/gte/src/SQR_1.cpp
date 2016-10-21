#include "TestGTE.h"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
      using namespace test;

      gte device;
      GTEtestbench tester(&device);

      tester.constructInstruction(1, 0, 0, 0, 0, SQR);

      tester.storeData(gte::IR1, 0x00002680);   // ir1 = 2.40625
      tester.storeData(gte::IR2, 0x00003A70);   // ir2 = 3.65234375
      tester.storeData(gte::IR3, 0x0000035A);   // ir3 = 0.20947265625

      tester.run();

      if (tester.getData(gte::MAC1) == 0x5CA4 &&
          tester.getData(gte::MAC2) == 0xD56F &&
          tester.getData(gte::MAC3) == 0x00B3 &&
          tester.getData(gte::IR1) == 0x5CA4 &&
          tester.getData(gte::IR2) == 0x7FFF && // saturated
          tester.getData(gte::IR3) == 0x00B3 &&
          tester.getControl(gte::FLAG) == 0x80800000)
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
