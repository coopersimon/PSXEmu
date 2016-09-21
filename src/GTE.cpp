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
      control_reg[FLAG].write(0);
      opcodes[GTECommand()](this);
}


// TODO: should the word be analysed inside the function?
// something to think about in these functions
word gte::A1(fixedPoint &input, unsigned integer, unsigned fraction)
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
      return input.getAsWord(integer, fraction);
}

word gte::A2(fixedPoint &input, unsigned integer, unsigned fraction)
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
      return input.getAsWord(integer, fraction);
}

word gte::A3(fixedPoint &input, unsigned integer, unsigned fraction)
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
      return input.getAsWord(integer, fraction);
}

word gte::B1(fixedPoint &input, unsigned integer, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 24, 1);
      }
      return input.getAsWord(integer, fraction); // TODO: are int/frac necessary here?
}

word gte::B2(fixedPoint &input, unsigned integer, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 23, 1);
      }
      return input.getAsWord(integer, fraction); // TODO: are int/frac necessary here?
}

word gte::B3(fixedPoint &input, unsigned integer, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 22, 1);
      }
      return input.getAsWord(integer, fraction); // TODO: are int/frac necessary here?
}

word gte::C1(fixedPoint &input)
{
      input.truncateFraction(0);
      if (input.checkSaturation(0xFF, true))
      {
            control_reg[FLAG].writeBits(1, 21, 1);
      }
      return input.getAsWord(8, 0);
}

word gte::C2(fixedPoint &input)
{
      input.truncateFraction(0);
      if (input.checkSaturation(0xFF, true))
      {
            control_reg[FLAG].writeBits(1, 20, 1);
      }
      return input.getAsWord(8, 0);
}

word gte::C3(fixedPoint &input)
{
      input.truncateFraction(0);
      if (input.checkSaturation(0xFF, true))
      {
            control_reg[FLAG].writeBits(1, 19, 1);
      }
      return input.getAsWord(8, 0);
}

/*word gte::D(fixedPoint &input)
{
      if (input.checkSaturation(0xFFFF, true))
      {
            control_reg[FLAG].writeBits(1, 18, 1);
      }
      return input;
}

fixedPoint gte::E(fixedPoint &input)
{
      return input;
}

fixedPoint gte::F(const fixedPoint &input)
{
      bool set_bit = input.checkBits(31);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 15, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 16, 1);
      }
      return input;
}

fixedPoint gte::G1(fixedPoint &input)
{
      if (input.checkSaturation(0x3FF, false))
      {
            control_reg[FLAG].writeBits(1, 14, 1);
      }
      return input;
}

fixedPoint gte::G2(fixedPoint &input)
{
      if (input.checkSaturation(0x3FF, false))
      {
            control_reg[FLAG].writeBits(1, 13, 1);
      }
      return input;
}

fixedPoint gte::Hx(fixedPoint &input)
{
      if (input.checkSaturation(0x1000, true))
      {
            control_reg[FLAG].writeBits(1, 12, 1);
      }
      return input;
}*/


/***** INSTRUCTIONS *****/

void gte::RESERVED()
{
      return;
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
      B1(intermediate, 28, 4); // ok this got uglier... TODO: maybe a different function is needed here?
      fixedPoint mac1 = rgbir + (ir0 * intermediate);
      data_reg[MAC1].write(A1(mac1, 28, 4));

      rgb = fixedPoint(data_reg[RGB].readByte1(), 0, false);
      ir = fixedPoint(data_reg[IR2].readLowerHalfword(), 12);
      fc = fixedPoint(control_reg[GFC].read(), 4);

      rgbir = rgb * ir;
      rgbir.truncateFraction(4);
      intermediate = fc - rgbir;
      B2(intermediate, 28, 4);
      fixedPoint mac2 = rgbir + (ir0 * intermediate);
      data_reg[MAC2].write(A2(mac2, 28, 4));

      rgb = fixedPoint(data_reg[RGB].readByte2(), 0, false);
      ir = fixedPoint(data_reg[IR3].readLowerHalfword(), 12);
      fc = fixedPoint(control_reg[BFC].read(), 4);

      rgbir = rgb * ir;
      rgbir.truncateFraction(4);
      intermediate = fc - rgbir;
      B3(intermediate, 28, 4);
      fixedPoint mac3 = rgbir + (ir0 * intermediate);
      // TODO: this line down to be rewritten with new fns
      data_reg[MAC3].write(A3(mac3, 28, 4));

      data_reg[IR1].writeLowerHalfword(B1(mac1, 12, 4));
      data_reg[IR2].writeLowerHalfword(B2(mac2, 12, 4));
      data_reg[IR3].writeLowerHalfword(B3(mac3, 12, 4));

      word crgb = data_reg[RGB].readByte3() << 24;
      crgb |= C1(mac1);
      crgb |= C2(mac2) << 8;
      crgb |= C3(mac3) << 16;

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
      if (shiftFraction())
      {
            fixedPoint ir1(data_reg[IR1].readLowerHalfword(), 12);
            fixedPoint mac1 = ir1 * ir1;
            data_reg[MAC1].write(A1(mac1, 20, 12));
            data_reg[IR1].writeLowerHalfword(B1(mac1, 4, 12));
            
            fixedPoint ir2(data_reg[IR2].readLowerHalfword(), 12);
            fixedPoint mac2 = ir2 * ir2;
            data_reg[MAC2].write(A2(mac2, 20, 12));
            data_reg[IR2].writeLowerHalfword(B2(mac2, 4, 12));
            
            fixedPoint ir3(data_reg[IR3].readLowerHalfword(), 12);
            fixedPoint mac3 = ir3 * ir3;
            data_reg[MAC3].write(A3(mac3, 20, 12));
            data_reg[IR3].writeLowerHalfword(B3(mac3, 4, 12));
      }

      else
      {
            fixedPoint ir1(data_reg[IR1].readLowerHalfword(), 0);
            fixedPoint mac1 = ir1 * ir1;
            data_reg[MAC1].write(A1(mac1, 32, 0));
            data_reg[IR1].writeLowerHalfword(B1(mac1, 16, 0));
            
            fixedPoint ir2(data_reg[IR2].readLowerHalfword(), 0);
            fixedPoint mac2 = ir2 * ir2;
            data_reg[MAC2].write(A2(mac2, 32, 0));
            data_reg[IR2].writeLowerHalfword(B2(mac2, 16, 0));
            
            fixedPoint ir3(data_reg[IR3].readLowerHalfword(), 0);
            fixedPoint mac3 = ir3 * ir3;
            data_reg[MAC3].write(A3(mac3, 32, 0));
            data_reg[IR3].writeLowerHalfword(B3(mac3, 16, 0));
      }
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
