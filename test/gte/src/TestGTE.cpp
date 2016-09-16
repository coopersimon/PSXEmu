// test functions for GTE

#include "TestGTE.h"

using namespace test;

void GTEtestbench::constructInstruction(unsigned shift_fraction, unsigned mult_matrix,
                                          unsigned mult_vector, unsigned trans_vector,
                                          unsigned lm, unsigned gte_opcode)
{
      instruction = 0;
      instruction |= (shift_fraction & 1) << 19;
      instruction |= (mult_matrix & 3) << 17;
      instruction |= (mult_vector & 3) << 15;
      instruction |= (trans_vector & 3) << 13;
      instruction |= (lm & 1) << 10;
      instruction |= (gte_opcode & 0x3F);
}

void GTEtestbench::constructInstruction(unsigned lm, unsigned gte_opcode)
{
      instruction = 0;
      instruction |= (lm & 1) << 10;
      instruction |= (gte_opcode & 0x3F);
}

void GTEtestbench::run()
{
      device->executeInstruction(instruction);
}

void GTEtestbench::storeControl(unsigned reg, word value)
{
      device->control_reg[reg].write(value);
}

word GTEtestbench::getControl(unsigned reg)
{
      return device->control_reg[reg].read();
}

void GTEtestbench::storeData(unsigned reg, word value)
{
      device->data_reg[reg].write(value);
}

word GTEtestbench::getData(unsigned reg)
{
      return device->data_reg[reg].read();
}
