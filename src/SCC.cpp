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
	// initialise register masks
	data_reg[INDX].setMask(0x80003F00);
	data_reg[RAND].setMask(0x00003F00); //TODO: rand register: this may be done differently
	data_reg[TLBL].setMask(0xFFFFFF00);
	data_reg[TLBH].setMask(0xFFFFFFC0);
	data_reg[CAUSE].setMask(0xB000FF7C);
	data_reg[CTXT].setMask(0xFFFFFFFC);
	data_reg[SR].setMask(0xF27FFF3F);
	data_reg[PRID].setMask(0x0000FFFF);
	data_reg[DCIC].setMask(0xEF800000);
	data_reg[DCIC].write(0xE0800000);

	// TODO: initialise function pointers
	/*instruction
	{
		
	};*/
}

void scc::writeDataReg(word data_in, unsigned dest_reg)
{
	if (dest_reg > 15)
		return;
	data_reg[dest_reg].write(data_in);
}

word scc::readDataReg(unsigned dest_reg) const
{
	if (dest_reg > 15)
		return 0;
	return data_reg[dest_reg].read();
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

