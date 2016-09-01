#include "SCC.h"

void SCCReg::writeBits(unsigned lower_bit, unsigned bits_to_write, word data_in)
{
	// create mask
	word mask = ((1 << bits_to_write) - 1) << lower_bit;
	// shift and mask data in
	data_in <<= lower_bit;
	data_in &= mask;
	// mask reg data and combine with data in
	data &= (mask ^ 0xFFFFFFFF);
	data |= data_in;
}


scc::scc()
{
	// initialise registers
	CAUSE = SCCReg(0, 0xB000FF7C);


	// initialise register pointers
	data_reg =
	{
		&INDX, &RAND, &TLBL, &BPC,
		&CTXT, &BDA, &PIDMASK, &DCIC,
		&BADV, &BDAM, &TLBH, &BPCM,
		&SR, &CAUSE, &EPC, &PRID
	};


	// initialise function pointers
	/*instruction
	{
		
	};*/
}

void scc::writeDataReg(word data_in, unsigned dest_reg)
{
	if (dest_reg > 15)
		return;
	data_reg[dest_reg]->write(data_in);
}

word scc::readDataReg(unsigned dest_reg) const
{
	if (dest_reg > 15)
		return 0;
	return data_reg[dest_reg]->read();
}


void TLBR()
{
}

void TLBWI()
{
}

void TLBWR()
{
}

void TLBP()
{
}

void RFE()
{
}

