#ifndef REGISTER_H
#define REGISTER_H

/*
 *	Register.h
 *
 *	Classes used to represent 32 and 16 bit registers
 *
 */

#include "Memory.h"

template <typename T>
class regCommon
{
protected:
	// data of register
	T data;

public:
      regCommon() : data(0) {};

	regCommon(T set_data) : data(set_data) {};

	inline T read() const
	{
		return data;
	}

	inline void write(T in)
	{
		data = in;
	}

};

class MIPSReg : public regCommon<word>
{
protected:
	word mask;

public:
	MIPSReg() : mask(0xFFFFFFFF), regCommon() {};
	MIPSReg(word set_data, word mask_in) : mask(mask_in), regCommon(set_data) {};

	void setMask(word mask_in)
	{
		mask = mask_in;
	}

	inline void write(word in)
	{
		data = in & mask;
	}	
};

typedef regCommon<word> reg32;
typedef regCommon<halfword> reg16;


#endif
