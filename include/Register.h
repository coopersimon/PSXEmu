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

	// options
	bool read_only;

public:
	regCommon() : read_only(false) {};

	regCommon(T set_data) : data(set_data), read_only(false) {};

	void readOnly()
	{
		read_only = true;
	}

	inline T read() const
	{
		return data;
	}

	inline void write(T in)
	{
		if (!read_only)
			data = in;
	}

	/*inline void increment(T in)
	{
		if (!read_only)
			data += in;
	}*/

};

class MIPSReg : public regCommon<word>
{
private:
	word mask;

public:
	MIPSReg() : mask(0xFFFFFFFF), regCommon(0) {};
	MIPSReg(word set_data, word mask_in) : mask(mask_in), regCommon(set_data) {};

	inline void setMask(word mask_in)
	{
		mask = mask_in;
	}

	inline void write(word in)
	{
		if (!read_only)
			data = in & mask;
	}
	
};

typedef regCommon<word> reg32;
typedef regCommon<halfword> reg16;


#endif
