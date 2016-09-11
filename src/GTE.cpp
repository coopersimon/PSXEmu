#include <GTE.h>

gte::gte()
{
      // register setup
      data_reg[SXYP].addFIFOReg(&data_reg[SXY1]);
      data_reg[SXYP].addFIFOReg(&data_reg[SXY0]);
      data_reg[SXYP].addFIFOMirror(&data_reg[SXY2]);

      data_reg[SZ3].addFIFOReg(&data_reg[SZ2]);
      data_reg[SZ3].addFIFOReg(&data_reg[SZ1]);
      data_reg[SZ3].addFIFOReg(&data_reg[SZ0]);

      data_reg[RGB2].addFIFOReg(&data_reg[RGB1]);
      data_reg[RGB2].addFIFOReg(&data_reg[RGB0]);

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
      // reset flag register
      opcodes[GTECommand()](this);
}


void gte::A1(fixedPoint &input)
{
      bool set_bit = input.checkBits(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 27, 1);
           return;
      }
      if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 30, 1);
           return;
      }
}

void gte::A2(fixedPoint &input)
{
      bool set_bit = input.checkBits(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 26, 1);
           return;
      }
      if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 29, 1);
           return;
      }
}

void gte::A3(fixedPoint &input)
{
      bool set_bit = input.checkBits(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 25, 1);
           return;
      }
      if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 28, 1);
           return;
      }
}

// the B1 values may need to set negative values to 0.
void gte::B1(fixedPoint &input)
{
      if (lm() && input.checkSign())
      {
            control_reg[FLAG].writeBits(1, 24, 1);
            return;
      }
      if (!lm() && input.checkBits(15))
      {
            control_reg[FLAG].writeBits(1, 24, 1);
            return;
      }
}

void gte::B2(fixedPoint &input)
{
      if (lm() && input.checkSign())
      {
            control_reg[FLAG].writeBits(1, 23, 1);
            return;
      }
      if (!lm() && input.checkBits(15))
      {
            control_reg[FLAG].writeBits(1, 23, 1);
            return;
      }
}

void gte::B3(fixedPoint &input)
{
      if (lm() && input.checkSign())
      {
            control_reg[FLAG].writeBits(1, 22, 1);
            return;
      }
      if (!lm() && input.checkBits(15))
      {
            control_reg[FLAG].writeBits(1, 22, 1);
            return;
      }
}





/***** INSTRUCTIONS *****/

void gte::RTPS()
{
      
}

void gte::RTPT()
{
}

void gte::MVMVA()
{
}

void gte::DCPL()
{
      fixedPoint rgb(data_reg[RGB].readByte0(), 8, 0);
      fixedPoint ir(data_reg[IR0].readLowerHalfword(), 3, 12);
      fixedPoint fc(data_reg[RFC].read(), 27, 4);
      fixedPoint rgbir = rgb * ir; // rgbir: 11.12
      fixedPoint intermediate = fc - rgbir; // intermediate: 27.12
      // now we check whether it's within 15 bits
      // truncate to 11.4
      // Lm_B1(intermediate)
      
}





