#ifndef SCC_H
#define SCC_H

/*
 *	SCC.h
 *
 *	Defines the SCC coprocessor
 *
 */

// includes
#include <Coprocessor.h>
#include <Register.h>
#include <vector>
#include <functional>

// forward declaration
class cpu;

// scc: cop0, deals with exception handling and memory management
class scc : public coprocessor
{
	// cpu needs to access registers
	friend cpu;

	// registers
	CoprocessorReg data_reg[16];

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

public:
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
