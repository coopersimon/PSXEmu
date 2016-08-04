#include "Coprocessor"

void scc::storeDataReg(word data_in, unsigned dest_reg) {
	if (dest_reg > 15)
		return;
	data_reg[dest_reg].write(data_in);
}

word scc::loadDataReg() {
	if (dest_reg > 15)
		return 0;
	return data_reg[dest_reg].read();
}


