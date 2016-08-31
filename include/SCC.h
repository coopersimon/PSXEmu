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

// register classes
// TODO: finish adding classes and define all functions
class indexReg : public MIPSReg
{
public:
	indexReg();
	void writeP(bool);
	void writeIndex(word);
	word readIndex();
}

class randomReg : public MIPSReg
{
public:
	randomReg();
	void decrement();
	word readRandom();
}

class TLBHigh : public MIPSReg
{
public:
	TLBHigh();
	void writeVPN(word);
	word readVPN();
	void writePID(word);
	word readPID();
}

class TLBLow : public MIPSReg
{
public:
	TLBLow();
	void writePFN(word);
	word readPFN();
	void nonCacheable(bool);
	void dirty(bool);
	void valid(bool);
	void global(bool);
}

class causeReg : public MIPSReg
{
public:
	causeReg();
	void writeBranchDelay(bool);
	void writeCoprocessorError(word);
	void writeInterruptsPending(word);
	void writeSoftwareInterrupts(word);
	// TODO: should exception codes use an enum (HINT: YES)
	void writeExceptionCode(word);
}

// this class might not be necessary
class exceptionPC : public MIPSReg
{
public:
	exceptionPC();
}

// this class might not be necessary
class BADV : public MIPSReg
{
}




// scc: cop0, deals with exception handling and memory management
class scc : public coprocessor
{
	std::vector<MIPSReg*> data_reg;
public:
	scc();
	~scc();

	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg);
	word readDataReg(unsigned source_reg) const;
	void executeInstruction(unsigned instruction);

private:
	// instructions
	void TLBR();
	void TLBWI();
	void TLBWR();
	void TLBP();
	void RFE();
};

#endif
