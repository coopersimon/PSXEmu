#ifndef COPROCESSOR_H
#define COPROCESSOR_H

/*
 *	Coprocessor.h
 *
 *	Defines the coprocessor classes which interface with the processor
 *
 */

// includes
#include "Register.h"
#include "PSException.h"

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


// gte: cop2, deals with vector and matrix transformations
class gte : public coprocessor
{
	reg32 data_reg[32];
	reg32 control_reg[32];

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
