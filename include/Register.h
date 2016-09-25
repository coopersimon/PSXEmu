#ifndef REGISTER_H
#define REGISTER_H

/*
 *	Register.h
 *
 *	Classes used to represent 32 bit registers
 *
 */

#include <Memory.h>

class regCommon
{
protected:
	// data of register
	word data;

public:
      regCommon() : data(0) {}

	regCommon(word set_data) : data(set_data) {}

	inline word read() const
	{
		return data;
	}

	inline void write(word in)
	{
		data = in;
	}

};

// used in cpu, like regCommon but with a mask
class MIPSReg : public regCommon
{
protected:
      // mask used for writing data
	word mask;

public:
	MIPSReg() : regCommon(), mask(0xFFFFFFFF) {}
	MIPSReg(word set_data, word mask_in) : regCommon(set_data), mask(mask_in) {}

	void setMask(word mask_in)
	{
		mask = mask_in;
	}

	inline void write(word in)
	{
		data = in & mask;
	}	
};

#endif
