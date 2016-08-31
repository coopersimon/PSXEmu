#include "SCC.h"

void scc::storeDataReg(word data_in, unsigned dest_reg)
{
	if (dest_reg > 15)
		return;
	data_reg[dest_reg].write(data_in);
}

word scc::loadDataReg(unsigned dest_reg)
{
	if (dest_reg > 15)
		return 0;
	return data_reg[dest_reg].read();
}

void scc::executeInstruction(unsigned instruction)
{
	switch (instruction)
	{
	case 1:
		TLBR();
		break;
	case 2:
		TLBWI();
		break;
	case 6:
		TLBWR();
		break;
	case 8:
		TLBP();
		break;
	case 16:
		RFE();
		break;
	default:
		throw new PSException; // TODO: which exception?
		break;
	}
}
