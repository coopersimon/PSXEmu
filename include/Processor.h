#ifndef PROCESSOR_H
#define PROCESSOR_H

/*
 *	Processor.h
 *
 *	Abstract classes to represent processors
 *
 */

// includes
#include "Register.h"

// type definitions
typedef reg32 MIPSReg;
typedef regData32 MIPSRegData;

// device: used for processors & co-processors (the CPU, GTE and SCC)
class device {
protected:
	// 32 bit registers
	MIPSReg *reg;

	device(unsigned register_count) {
		reg = new MIPSReg[register_count];
	}

	~device() {
		delete[] reg;
	}

};

#endif
