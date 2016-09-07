#include "Memory.h"
#include "CPU.h"
#include <iostream>

int main()
{
	// create cpu
	memoryInterface *mem = new RAMImpl(8);
	// set reg 1 & 2 to 0x8000,0000 (LUI)
	mem->writeWordLittle(0, 0x3C018000);
	mem->writeWordLittle(4, 0x3C028000);
	// add reg 1 & 2 to force overflow
	mem->writeWordLittle(8, 0x00220820);

	mem->print();
	cpu processor(mem);
	processor.stepCPU();
	processor.stepCPU();
	processor.stepCPU();
	std::cout << "if you saw 12 then success!\n";
	return 0;
}
