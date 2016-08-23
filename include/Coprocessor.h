#ifndef COPROCESSOR_H
#define COPROCESSOR_H

/*
 *	Coprocessor.h
 *
 *	Defines the coprocessor classes which interface with the processor
 *
 */

// includes
#include "Processor.h"
#include "Register.h"
#include "Exception.h"

// generic empty coprocessor class
class coprocessor
{
public:
	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg) { throw new cpuException; }
	word readDataReg(unsigned source_reg) { throw new cpuException; }
	void writeControlReg(word data_in, unsigned dest_reg) { throw new cpuException; }
	word readControlReg(unsigned source_reg) { throw new cpuException; }
	void executeInstruction(unsigned instruction) { throw new cpuException; }
};


// scc: cop0, deals with exception handling and memory management
class scc : public coprocessor
{
	MIPSReg data_reg[16];
public:
	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg);
	word readDataReg(unsigned source_reg);
	void executeInstruction(unsigned instruction);

private:
	// instructions
	void TLBR();
	void TLBWI();
	void TLBWR();
	void TLBP();
	void RFE();
};


// gte: cop2, deals with vector and matrix transformations
class gte : public coprocessor
{
	MIPSReg data_reg[32];
	MIPSReg control_reg[32];

public:
	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg);
	word readDataReg(unsigned source_reg);
	void writeControlReg(word data_in, unsigned dest_reg);
	word readControlReg(unsigned source_reg);
	void executeInstruction(unsigned instruction);

private:
	// instructions...
};

#endif
