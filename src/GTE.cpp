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


fixedPoint gte::A1(const fixedPoint &input)
{
      bool set_bit = input.checkBits(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 27, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 30, 1);
      }
      return input;
}

fixedPoint gte::A2(const fixedPoint &input)
{
      bool set_bit = input.checkBits(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 26, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 29, 1);
      }
      return input;
}

fixedPoint gte::A3(const fixedPoint &input)
{
      bool set_bit = input.checkBits(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 25, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 28, 1);
      }
      return input;
}

fixedPoint gte::B1(fixedPoint &input)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 24, 1);
      }
      return input;
}

fixedPoint gte::B2(fixedPoint &input)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 23, 1);
      }
      return input;
}

void gte::B3(fixedPoint &input)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 22, 1);
            return;
      }
}

void gte::C1(fixedPoint &input)
{
      if (input.checkSaturation(0xFF, true))
      {
            control_reg[FLAG].writeBits(1, 21, 1);
      }
}

void gte::C2(fixedPoint &input)
{
      if (input.checkSaturation(0xFF, true))
      {
            control_reg[FLAG].writeBits(1, 20, 1);
      }
}

void gte::C3(fixedPoint &input)
{
      if (input.checkSaturation(0xFF, true))
      {
            control_reg[FLAG].writeBits(1, 19, 1);
      }
}

void gte::D(fixedPoint &input)
{
      if (input.checkSaturation(0xFFFF, true))
      {
            control_reg[FLAG].writeBits(1, 18, 1);
      }
}

void gte::E(fixedPoint &input)
{
}

void gte::F(fixedPoint &input)
{
      bool set_bit = input.checkBits(31);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 15, 1);
           return;
      }
      if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 16, 1);
           return;
      }
}

void gte::G1(fixedPoint &input)
{
      if (input.checkSaturation(0x3FF, false))
      {
            control_reg[FLAG].writeBits(1, 14, 1);
      }
}

void gte::G2(fixedPoint &input)
{
      if (input.checkSaturation(0x3FF, false))
      {
            control_reg[FLAG].writeBits(1, 13, 1);
      }
}

void gte::Hx(fixedPoint &input)
{
      if (input.checkSaturation(0x1000, true))
      {
            control_reg[FLAG].writeBits(1, 12, 1);
      }
}


/***** INSTRUCTIONS *****/

void gte::RESERVED()
{
}


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
      fixedPoint rgb(data_reg[RGB].readByte0(), 0, false);
      fixedPoint ir(data_reg[IR1].readLowerHalfword(), 12);
      fixedPoint fc(control_reg[RFC].read(), 4);
      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), 12);
      
      fixedPoint rgbir = rgb * ir; // rgbir: 11.12
      rgbir.truncateFraction(4); // this shouldn't really be needed. subtraction might take care of it
      fixedPoint intermediate = fc - rgbir; // intermediate: 27.4
      B1(intermediate);
      fixedPoint mac1 = rgbir + (ir0 * intermediate);
      data_reg[MAC1].write(mac1.getAsWord(28, 4)); // maybe get as word should also truncate?

      rgb = fixedPoint(data_reg[RGB].readByte1(), 0, false);
      ir = fixedPoint(data_reg[IR2].readLowerHalfword(), 12);
      fc = fixedPoint(control_reg[GFC].read(), 4);
      
      rgbir = rgb * ir;
      rgbir.truncateFraction(4);
      intermediate = fc - rgbir;
      B2(intermediate);
      fixedPoint mac2 = rgbir + (ir0 * intermediate);
      data_reg[MAC2].write(mac2.getAsWord(28, 4));

      rgb = fixedPoint(data_reg[RGB].readByte2(), 0, false);
      ir = fixedPoint(data_reg[IR3].readLowerHalfword(), 12);
      fc = fixedPoint(control_reg[BFC].read(), 4);
      
      rgbir = rgb * ir;
      rgbir.truncateFraction(4);
      intermediate = fc - rgbir;
      B3(intermediate);
      fixedPoint mac3 = rgbir + (ir0 * intermediate);
      data_reg[MAC3].write(mac3.getAsWord(28, 4));

      mac1.truncateFraction(4);
      B1(mac1);
      data_reg[IR1].writeLowerHalfword(mac1.getAsWord(12, 4));
      mac2.truncateFraction(4);
      B2(mac2);
      data_reg[IR2].writeLowerHalfword(mac2.getAsWord(12, 4));
      mac3.truncateFraction(4);
      B3(mac3);
      data_reg[IR3].writeLowerHalfword(mac3.getAsWord(12, 4));

      word crgb = data_reg[RGB].readByte3() << 24;
      C1(mac1);
      crgb |= mac1.getAsWord(8, 0);
      C2(mac2);
      crgb |= mac2.getAsWord(8, 0) << 8;
      C3(mac3);
      crgb |= mac3.getAsWord(8, 0) << 16;

      data_reg[RGB2].write(crgb);
}

void gte::DPCS()
{
}

void gte::DPCT()
{
}

void gte::INTPL()
{
}

void gte::SQR()
{
}

void gte::NCS()
{
}

void gte::NCT()
{
}

void gte::NCDS()
{
}

void gte::NCDT()
{
}

void gte::NCCS()
{
}

void gte::NCCT()
{
}

void gte::CDP()
{
}

void gte::CC()
{
}

void gte::NCLIP()
{
}

void gte::AVSZ3()
{
}

void gte::AVSZ4()
{
}

void gte::OP()
{
}

void gte::GPF()
{
}

void gte::GPL()
{
}
