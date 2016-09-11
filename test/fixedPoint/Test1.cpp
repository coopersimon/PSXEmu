#include <FixedPointMaths.h>
#include <iostream>
#include <iomanip>

int main()
{
      // multing together 10 and 5.00
      // we should get 50.00

      fixedPoint a(0x0000000A, 16, 0);
      fixedPoint b(0x00000014, 14, 2);

      fixedPoint c = a * b;
      std::cout << std::hex << c.getAsWord(14, 2) << std::endl;

      return 0;
}
