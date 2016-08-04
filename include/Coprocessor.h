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

class coprocessor {
public:	
	void storeDataReg(word data_in, unsigned dest_reg) { throw new cpuException; };
	word loadDataReg(unsigned source_reg) { throw new cpuException; };
	void storeControlReg(word data_in, unsigned dest_reg) { throw new cpuException; };
	word loadControlReg(unsigned source_reg) { throw new cpuException; };
};


// scc: cop0
class scc : public coprocessor {
	MIPSReg data_reg[16];
public:
	void storeDataReg(word data_in, unsigned dest_reg);
	word loadDataReg(unsigned source_reg);
};


// gte: cop2
class gte : public coprocessor {
	MIPSReg data_reg[32];
	MIPSReg control_reg[32];
public:
	void storeDataReg(word data_in, unsigned dest_reg);
	word loadDataReg(unsigned source_reg);
	void storeControlReg(word data_in, unsigned dest_reg);
	word loadControlReg(unsigned source_reg);
};

#endif
