#include <GTE.h>


gte::gte()
{
      // function pointers
      opcodes = {
            &gte::RESERVED, &gte::RTPS, &gte::RESERVED, &gte::RESERVED,
            &gte::RESERVED, &gte::RESERVED, &gte::NCLIP, &gte::RESERVED,
            &gte::RESERVED, &gte::RESERVED, &gte::RESERVED, &gte::RESERVED,
            &gte::OP, &gte::RESERVED, &gte::RESERVED, &gte::RESERVED,
            &gte::DPCS, &gte::INTPL, &gte::MVMVA, &gte::NCDS,
            &gte::CDP, &gte::RESERVED, &gte::NCDT, &gte::RESERVED,
            &gte::RESERVED, &gte::RESERVED, &gte::RESERVED, &gte::NCCS,
            &gte::CC, &gte::RESERVED, &gte::NCS, &gte::RESERVED,
            &gte::NCT, &gte::RESERVED, &gte::RESERVED, &gte::RESERVED,
            &gte::RESERVED, &gte::RESERVED, &gte::RESERVED, &gte::RESERVED,
            &gte::SQR, &gte::DCPL, &gte::DPCT, &gte::RESERVED, 
            &gte::RESERVED, &gte::AVSZ3, &gte::AVSZ4, &gte::RESERVED,
            &gte::RTPT, &gte::RESERVED, &gte::RESERVED, &gte::RESERVED,
            &gte::RESERVED, &gte::RESERVED, &gte::RESERVED, &gte::RESERVED,
            &gte::RESERVED, &gte::RESERVED, &gte::RESERVED, &gte::RESERVED,
            &gte::RESERVED, &gte::GPF, &gte::GPL, &gte::NCCT
      };
}

void gte::writeDataReg(word data_in, unsigned dest_reg)
{
      data_reg[dest_reg].write(data_in);
}

word gte::readDataReg(unsigned source_reg) const
{
      return data_reg[source_reg].read();
}

void gte::writeControlReg(word data_in, unsigned dest_reg)
{
      control_reg[dest_reg].write(data_in);
}

word gte::readControlReg(unsigned source_reg) const
{
      return control_reg[source_reg].read();
}

void gte::executeInstruction(unsigned instruction_in)
{
      instruction = instruction_in;
      opcodes[GTECommand()](this);
}

/***** INSTRUCTIONS *****/

void gte::RTPS()
{
      
}







