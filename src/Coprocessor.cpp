#include <Coprocessor.h>

void CoprocessorReg::writeBits(word data_in, unsigned lower_bit, unsigned bits_to_write)
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
