#include <GLGPU.h>
#include <iostream>
#include <ctime>

int main()
{
      glgpu device;

      srand(time(NULL));

      while (device.refreshWindow())
      {
            unsigned xy1 = ((rand() % 240) << 16) | (rand() % 320);
            unsigned xy2 = ((rand() % 240) << 16) | (rand() % 320);
            unsigned xy3 = ((rand() % 240) << 16) | (rand() % 320);
            unsigned c2 = ((rand() % 240) << 16) | (rand() % 320);
            unsigned c3 = ((rand() % 240) << 16) | (rand() % 320);
            //unsigned command = 0x32000000 | (rand() & 0xFFFFFF);
            unsigned command = rand();

            device.writeWord(0, command);
            device.writeWord(0, xy1);
            device.writeWord(0, c2);
            device.writeWord(0, xy2);
            device.writeWord(0, c3);
            device.writeWord(0, xy3);

            /*device.writeWord(0, 0x00000000);
            device.writeWord(0, 0x00E000A0);
            device.writeWord(0, 0x00000140);

            device.writeWord(0, 0x200000FF);
            device.writeWord(0, 0x00000000);
            device.writeWord(0, 0x00100010);
            device.writeWord(0, 0x00000020);

            device.writeWord(0, 0x2000FF00);
            device.writeWord(0, 0x00000120);
            device.writeWord(0, 0x00100130);
            device.writeWord(0, 0x00000140);

            device.writeWord(0, 0x20FF0000);
            device.writeWord(0, 0x00C00090);
            device.writeWord(0, 0x00D000A0);
            device.writeWord(0, 0x00C000B0);*/
      }
}
