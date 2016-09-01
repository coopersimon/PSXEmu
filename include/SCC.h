#ifndef SCC_H
#define SCC_H

/*
 *	SCC.h
 *
 *	Defines the SCC coprocessor
 *
 */

// includes
#include "Coprocessor.h"
#include "Register.h"
#include <vector>
#include <functional>

class cpu;

class SCCReg : public MIPSReg
{
public:
	SCCReg() : MIPSReg() {}
	SCCReg(word set_data, word mask_in) : MIPSReg(set_data, mask_in) {}
	void writeBits(unsigned lower_bit, unsigned bits_to_write, word data_in);
};

// scc: cop0, deals with exception handling and memory management
class scc : public coprocessor
{
	// cpu needs to access registers
	friend cpu;

	// generic pointers to all the registers
	SCCReg data_reg[16];

	// function pointer
	std::vector<std::function<void(scc*)>> instruction;
	
public:
	scc();

	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg);
	word readDataReg(unsigned source_reg) const;

private:
	// instructions
	void TLBR();
	void TLBWI();
	void TLBWR();
	void TLBP();
	void RFE();

	enum
	{
		INDX = 0,
		RAND,
		TLBL,
		BPC,
		CTXT,
		BDA,
		PIDMASK,
		DCIC,
		BADV,
		BDAM,
		TLBH,
		BPCM,
		SR,
		CAUSE,
		EPC,
		PRID,
		ERREG
	};
};

#endif
