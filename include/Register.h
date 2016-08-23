#ifndef REGISTER_H
#define REGISTER_H

/*
 *	Register.h
 *
 *	Classes used to represent 32 and 16 bit registers
 *
 */

#include <cstdint>

template <typename T>
class regCommon
{
	// data of register
	T data;
	// options
	bool set_zero : 1;
	bool read_only : 1;

public:
	regCommon() : set_zero(false), read_only(false) {};

	regCommon(T set_data) : data(set_data), set_zero(false), read_only(false) {};

	void set_vars(bool sz, bool ro)
	{
		set_zero = sz;
		read_only = ro;
	}

	inline T read()
	{
		if (!set_zero)
			return data;
		else
			return 0;
	}

	inline void write(T in)
	{
		if (!read_only)
			data = in;
	}

	inline void increment(T in)
	{
		if (!read_only)
			data += in;
	}
};

/*template <typename T>
regCommon<T> &operator=(regCommon<T> lhs, regCommon<T> rhs) {
	T data;
	rhs.read(data);
	lhs.write(data);
	return lhs;
}

template <typename T>
regCommon<T> operator+(regCommon<T> lhs, regCommon<T> rhs) {
	T ldata, rdata;
	lhs.read(ldata);
	rhs.read(rdata);
	ldata += rdata;
	lhs.write(ldata);
	return lhs;
}*/


typedef uint32_t regData32;
typedef uint16_t regData16;

typedef regCommon<regData32> reg32;
typedef regCommon<regData16> reg16;


#endif
