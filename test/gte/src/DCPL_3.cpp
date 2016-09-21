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

      // testing FIFO

      tester.run();
      tester.run(); // rgb2->rgb1

      if (tester.getData(gte::RGB1) == 0x00D2D290)
            std::cout << argv[0] << " Pass" << std::endl;
      else
      {
            std::cout << argv[0] << " Fail" << std::endl;
            std::cout << "RGB2: " << std::hex << tester.getData(gte::RGB2) << std::endl;
            std::cout << "RGB1: " << std::hex << tester.getData(gte::RGB1) << std::endl;
      }
}
