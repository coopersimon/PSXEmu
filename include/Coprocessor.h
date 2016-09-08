#ifndef COPROCESSOR_H
#define COPROCESSOR_H

/*
 *	Coprocessor.h
 *
 *    Decleares the coprocessor classes which interface with the processor
 *
 */

// includes
#include <Register.h>
#include <PSException.h>

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

#endif
