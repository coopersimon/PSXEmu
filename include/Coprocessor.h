#ifndef COPROCESSOR_H
#define COPROCESSOR_H

/*
 *	Coprocessor.h
 *
 *    Declares the base coprocessor class which interfaces with the processor
 *    Also includes the register used with it
 */

// includes
#include <Register.h>
#include <PSException.h>

// generic empty coprocessor class
class coprocessor
{
public:
	// register transfers
	virtual void writeDataReg(word data_in, unsigned dest_reg) { throw cpuException(); }
	virtual word readDataReg(unsigned source_reg) const { throw cpuException(); }
	virtual void writeControlReg(word data_in, unsigned dest_reg) { throw cpuException(); }
	virtual word readControlReg(unsigned source_reg) const { throw cpuException(); }
      // coprocessor instruction
	virtual void executeInstruction(unsigned instruction) { throw cpuException(); }
};

// adds read only & bit field writing
class CoprocessorReg : public MIPSReg
{
      bool read_only;

public:
	CoprocessorReg() : MIPSReg(), read_only(false) {}
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

#endif
