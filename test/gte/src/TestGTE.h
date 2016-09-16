#ifndef TEST_GTE_H
#define TEST_GTE_H

// used for testing GTE

#include <GTE.h>
#include <Memory.h>

namespace test
{

class GTEtestbench
{
private:
      gte *device;

      word instruction;
      
public:
      GTEtestbench(gte *test_device) : device(test_device), instruction(0) {}

      // construct instruction
      void constructInstruction(unsigned shift_fraction, unsigned mult_matrix, unsigned mult_vector, unsigned trans_vector, unsigned lm, unsigned gte_opcode); // for instructions that need it

      void constructInstruction(unsigned lm, unsigned gte_opcode); // for most instructions

      // run instruction
      void run();

      // control registers
      void storeControl(unsigned reg, word value);
      word getControl(unsigned reg);

      // data registers
      void storeData(unsigned reg, word value);
      word getData(unsigned reg);
};

enum
{
      RTPS = 0x1,
      NCLIP = 0x6,
      OP = 0xC,
      DPCS = 0x10,
      INTPL = 0x11,
      MVMVA = 0x12,
      NCDS = 0x13,
      CDP = 0x14,
      NCDT = 0x16,
      NCCS = 0x1B,
      CC = 0x1C,
      NCS = 0x1E,
      NCT = 0x20,
      SQR = 0x28,
      DCPL = 0x29,
      DPCT = 0x2A,
      AVSZ3 = 0x2D,
      AVSZ4 = 0x2E,
      RTPT = 0x30,
      GPF = 0x3D,
      GPL = 0x3E,
      NCCT = 0x3F
};

}; // end namespace test

#endif
