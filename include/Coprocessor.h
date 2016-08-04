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


// still unsure about these. coprocessor = abstract class? with defined interface? fuck knows
	// should it inherit from "device" class? should device even exist?
class coprocessor {
	

public:	
	



};



// scc: cop0
class scc : public coprocessor {
public:
	scc() : device(32) {}; // data registers



};



// gte: cop2
class gte : public coprocessor {

};









#endif
