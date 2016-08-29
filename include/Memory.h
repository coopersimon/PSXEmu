#ifndef MEMORY_H
#define MEMORY_H

/*
 *	Memory.h
 *
 *	Used for all types of memory in the system:
 *		RAM, ROM, VRAM & potentially cache
 */

// includes
#include <cstdint>
#include <list>

// types TODO: potentially add an address type
typedef uint8_t		byte;
typedef int8_t		s_byte;
typedef uint16_t	halfword;
typedef int16_t		s_halfword;
typedef uint32_t	word;
typedef int32_t		s_word;
typedef uint64_t	doubleword;
typedef int64_t		s_doubleword;

// this represents any kind of memory
template<typename T>
class rawMemory
{
	// data stored in bytes
	T* data;

protected:
	rawMemory(unsigned data_size) { data = new T[data_size]; }
	~rawMemory() { delete[] data; }

	inline void read(unsigned address, T &out) { out = data[address]; }
	inline void write(unsigned address, T in) { data[address] = in; }
	// reading and writing multiple might be a bit overkill in the primitive raw memory class
	inline void readMultiple(unsigned address, T *out, unsigned amount) { for (int i = 0; i < amount; i++) out[i] = data[address + i]; }
	inline void writeMultiple(unsigned address, T *in, unsigned amount) { for (int i = 0; i < amount; i++) data[address + i] = in[i]; }
};

// this represents the public memory API for loading different mem sizes.
// basically used when load/storing within the CPU
class memoryInterface
{
public:
	virtual byte readByte(unsigned address) = 0;
	virtual void writeByte(unsigned address, byte in) = 0;

	virtual halfword readHalfwordLittle(unsigned address) = 0;
	virtual void writeHalfwordLittle(unsigned address, halfword in) = 0;
	virtual halfword readHalfwordBig(unsigned address) = 0;
	virtual void writeHalfwordBig(unsigned address, halfword in) = 0;

	virtual word readWordLittle(unsigned address) = 0;
	virtual void writeWordLittle(unsigned address, word in) = 0;
	virtual word readWordBig(unsigned address) = 0;
	virtual void writeWordBig(unsigned address, word in) = 0;

};

// the bus contains a list of pointers to memory devices so they can be accessed from within processor classes
class memBus : public memoryInterface
{
	// list of memory devices and their addressing ranges
	struct memoryPointer
	{
		unsigned address_start;
		unsigned address_end;
		memoryInterface *memory_device;
	};

	std::list<memoryPointer> bus_list;

public:
	// the memory devices are not owned by the bus so we don't have to delete them afterwards
	void mountMemory(memoryInterface *memory_device, unsigned address_start, unsigned address_end)
		{ bus_list.push_back({ address_start, address_end, memory_device }); }

	// all of these look up the address on the bus, translate it and operate on memory
	byte readByte(unsigned address);
	void writeByte(unsigned address, byte in);

	halfword readHalfwordLittle(unsigned address);
	void writeHalfwordLittle(unsigned address, halfword in);
	halfword readHalfwordBig(unsigned address);
	void writeHalfwordBig(unsigned address, halfword in);

	word readWordLittle(unsigned address);
	void writeWordLittle(unsigned address, word in);
	word readWordBig(unsigned address);
	void writeWordBig(unsigned address, word in);

};

// this represents RAM
class RAMImpl : public rawMemory<byte>, public memoryInterface
{
	unsigned addr_bits;

	inline unsigned maskAddress(unsigned address)
	{
		return address & ((1 << addr_bits) | ((1 << addr_bits) - 1));
	}

public:
	// input number of bits for addressing. memory is stored in terms of data size.
	RAMImpl(unsigned addr_bits_in) : rawMemory(1 << addr_bits_in), addr_bits(addr_bits_in) {};

	byte readByte(unsigned address);
	void writeByte(unsigned address, byte in);

	halfword readHalfwordLittle(unsigned address);
	void writeHalfwordLittle(unsigned address, halfword in);
	halfword readHalfwordBig(unsigned address);
	void writeHalfwordBig(unsigned address, halfword in);

	word readWordLittle(unsigned address);
	void writeWordLittle(unsigned address, word in);
	word readWordBig(unsigned address);
	void writeWordBig(unsigned address, word in);
};

#endif
