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

class CoprocessorReg : public MIPSReg
{
      bool read_only;

public:
	CoprocessorReg() : MIPSReg(), read_only(false){}
	CoprocessorReg(word set_data, word mask_in) : MIPSReg(set_data, mask_in), read_only(false) {}

      void readOnly()
      {
            read_only = true;
      }

      inline void write(word in) // for your standard writes
      {
            if (!read_only)
                  data = in & mask;
      }

      // write DATA_IN from LOWER_BIT for BITS_TO_WRITE
	void writeBits(word data_in, unsigned lower_bit, unsigned bits_to_write);

      // write DATA_IN from 0 for 32 bits
      void writeBits(word data_in) { data = data_in; }
};

// generic empty coprocessor class
class coprocessor
{
public:
	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg) { throw cpuException(); }
	word readDataReg(unsigned source_reg) const { throw cpuException(); }
	void writeControlReg(word data_in, unsigned dest_reg) { throw cpuException(); }
	word readControlReg(unsigned source_reg) const { throw cpuException(); }
	void executeInstruction(unsigned instruction) { throw cpuException(); }
};

#endif
