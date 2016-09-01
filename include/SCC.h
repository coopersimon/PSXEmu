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

// register classes
// TODO: finish adding classes and define all functions
/*class indexReg : public MIPSReg
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

class contextReg : public MIPSReg
{
public:
	contextReg();
	void BADV(word);
	void PTEBase(word);
}
*/

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
	// generic pointers to all the registers
	std::vector<SCCReg*> data_reg;

	// the registers
	SCCReg INDX;
	SCCReg RAND;
	SCCReg TLBL;
	SCCReg BPC;
	SCCReg CTXT;
	SCCReg BDA;
	SCCReg PIDMASK;
	SCCReg DCIC;
	SCCReg BADV;
	SCCReg BDAM;
	SCCReg TLBH;
	SCCReg BPCM;
	SCCReg SR;
	SCCReg CAUSE;
	SCCReg EPC;
	SCCReg PRID;

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

	friend cpu;
};

#endif
