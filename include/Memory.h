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
#include <vector>

// types TODO: potentially add an address type
typedef uint8_t   byte;
typedef int8_t    s_byte;
typedef uint16_t  halfword;
typedef int16_t   s_halfword;
typedef uint32_t  word;
typedef int32_t   s_word;
typedef uint64_t  doubleword;
typedef int64_t   s_doubleword;


// this represents any kind of memory
template<typename T>
class rawMemory
{
      // data stored in bytes
      T* data;

protected:
      rawMemory(unsigned data_size) { data = new T[data_size]; }
      ~rawMemory() { delete[] data; }
      
      inline T read(unsigned address) { return data[address]; }
      inline void write(unsigned address, T in) { data[address] = in; }
      // reading and writing multiple might be a bit overkill in the primitive raw memory class
      //inline void readMultiple(unsigned address, T *out, unsigned amount) { for (int i = 0; i < amount; i++) out[i] = data[address + i]; }
      //inline void writeMultiple(unsigned address, T *in, unsigned amount) { for (int i = 0; i < amount; i++) data[address + i] = in[i]; }
};


// this represents the public memory API for loading/storing memory.
class memoryInterface
{
public:
      virtual ~memoryInterface() {}

      // virtual word checkWord(unsigned address) {return readWord(address)}; // for writing bytes/halfwords, ensuring data doesnt get destroyed
      virtual word readWord(unsigned address) = 0;
      virtual void writeWord(unsigned address, word in) = 0;
};


// TODO: double check if inhereiting from interface is correct call
// TODO: should this be in this header? or should it be closer to CPU?
// the bus contains a list of pointers to memory devices so they can be accessed from within processor classes
class memBus : public memoryInterface
{
      // list of memory devices and their addressing ranges
      struct memoryPointer
      {
      	unsigned address_start;
      	unsigned address_end;
      	memoryInterface *memory_device;
      	memoryPointer(unsigned addr_start, unsigned  addr_end, memoryInterface *mem_dev) :
                  address_start(addr_start), address_end(addr_end), memory_device(mem_dev) {} 
      };

      std::vector<memoryPointer> bus_list;
      
public:
      // the memory devices are not owned by the bus so we don't have to delete them afterwards
      void mountMemory(memoryInterface *memory_device, unsigned address_start, unsigned address_end)
      	{ bus_list.push_back(memoryPointer(address_start, address_end, memory_device)); }
      
      // all of these look up the address on the bus and return the appropriate device
      int find(unsigned address);
     
      // memoryInterface functions
      word readWord(unsigned address) override;
      void writeWord(unsigned address, word in) override;
};


// this represents RAM
class RAMImpl : public rawMemory<word>, public memoryInterface
{
      unsigned addr_bits;
      
      inline unsigned maskAddress(unsigned address)
      {
      	return address & ((1 << addr_bits) | ((1 << addr_bits) - 1));
      }
      
public:
      // input number of bits for addressing at word level
      RAMImpl(unsigned addr_bits_in) : rawMemory(1 << (addr_bits_in)), addr_bits(addr_bits_in) {}
      
      word readWord(unsigned address) override;
      void writeWord(unsigned address, word in) override;
};


#endif
