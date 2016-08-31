#include "Memory.h"
#include "CPU.h"
#include <iostream>

int main()
{
	// create cpu
	memoryInterface *mem = new RAMImpl(8);
	mem->writeWordLittle(0, 0x60000000);
	cpu processor(mem);
	processor.stepCPU();
	std::cout << "if you saw 10 then success!\n";
	return 0;
}
