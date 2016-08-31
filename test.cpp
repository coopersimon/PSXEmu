#include "Memory.h"
#include "CPU.h"
#include <iostream>

int main()
{
	memoryInterface *mem = new RAMImpl(8);
	mem->writeWordLittle(0, 0x60000000);
	cpu processor(mem);
	processor.stepCPU();
	std::cout << "success!\n";
	return 0;
}
