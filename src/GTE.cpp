#include <GTE.h>
#include <iostream>

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
      // write checksum
      if (control_reg[FLAG].read() & 0x7FC7E000)
            control_reg[FLAG].writeBits(1, 31, 1);
}


// TODO: should the word be analysed inside the function?
// something to think about in these functions
// TODO: probably dont need int AND frac
word gte::A1(fixedPoint &input, unsigned fraction)
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
      return input.getAsWord(32-fraction, fraction);
}

word gte::A2(fixedPoint &input, unsigned fraction)
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
      return input.getAsWord(32-fraction, fraction);
}

word gte::A3(fixedPoint &input, unsigned fraction)
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
      return input.getAsWord(32-fraction, fraction);
}

word gte::B1(fixedPoint &input, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 24, 1);
      }
      return input.getAsWord(16-fraction, fraction);
}

word gte::B2(fixedPoint &input, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 23, 1);
      }
      return input.getAsWord(16-fraction, fraction);
}

word gte::B3(fixedPoint &input, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 22, 1);
      }
      return input.getAsWord(16-fraction, fraction);
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
}*/

word gte::F(fixedPoint &input)
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
      return input.getAsWord(32, 0);
}

/*fixedPoint gte::G1(fixedPoint &input)
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
      /***** INPUTS *****/
      fixedPoint ir1(data_reg[IR1].readLowerHalfword(), 12);
      fixedPoint ir2(data_reg[IR2].readLowerHalfword(), 12);
      fixedPoint ir3(data_reg[IR3].readLowerHalfword(), 12);

      fixedPoint rfc(control_reg[RFC].read(), 4);
      fixedPoint gfc(control_reg[GFC].read(), 4);
      fixedPoint bfc(control_reg[BFC].read(), 4);

      fixedPoint r(data_reg[RGB].readByte0(), 0, false);
      fixedPoint g(data_reg[RGB].readByte1(), 0, false);
      fixedPoint b(data_reg[RGB].readByte2(), 0, false);

      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), 12);

      /***** CALCULATION *****/
      fixedPoint rir = r * ir1; // rgbir: 11.12
      rir.truncateFraction(4); // this shouldn't really be needed. subtraction might take care of it
      fixedPoint intermediate = rfc - rir; // intermediate: 27.4
      B1(intermediate, 4); // ok this got uglier... TODO: maybe a different function is needed here?
      fixedPoint mac1 = rir + (ir0 * intermediate);

      fixedPoint gir = g * ir2;
      gir.truncateFraction(4);
      intermediate = gfc - gir;
      B2(intermediate, 4);
      fixedPoint mac2 = gir + (ir0 * intermediate);

      fixedPoint bir = b * ir3;
      bir.truncateFraction(4);
      intermediate = bfc - bir;
      B3(intermediate, 4);
      fixedPoint mac3 = bir + (ir0 * intermediate);

      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, 4));
      data_reg[MAC2].write(A2(mac2, 4));
      data_reg[MAC3].write(A3(mac3, 4));
      data_reg[IR1].writeLowerHalfword(B1(mac1, 4));
      data_reg[IR2].writeLowerHalfword(B2(mac2, 4));
      data_reg[IR3].writeLowerHalfword(B3(mac3, 4));

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
      /***** INPUTS *****/
      fixedPoint ir1(data_reg[IR1].readLowerHalfword(), 12);
      fixedPoint ir2(data_reg[IR2].readLowerHalfword(), 12);
      fixedPoint ir3(data_reg[IR3].readLowerHalfword(), 12);

      fixedPoint rfc(control_reg[RFC].read(), 4);
      fixedPoint gfc(control_reg[GFC].read(), 4);
      fixedPoint bfc(control_reg[BFC].read(), 4);

      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), 12);

      /***** CALCULATION *****/
      fixedPoint intermediate = rfc - ir1; // TODO investigate subtraction & truncation
      B1(intermediate, 4); // TODO: we need 2 versions of B1/2/3
      fixedPoint mac1 = ir1 + (ir0 * intermediate);

      intermediate = gfc - ir2;
      B2(intermediate, 4);
      fixedPoint mac2 = ir2 + (ir0 * intermediate);

      intermediate = bfc - ir3;
      B3(intermediate, 4);
      fixedPoint mac3 = ir3 + (ir0 * intermediate);
      
      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, 4));
      data_reg[MAC2].write(A2(mac2, 4));
      data_reg[MAC3].write(A3(mac3, 4));
      data_reg[IR1].writeLowerHalfword(B1(mac1, 4));
      data_reg[IR2].writeLowerHalfword(B2(mac2, 4));
      data_reg[IR3].writeLowerHalfword(B3(mac3, 4));

      word crgb = data_reg[RGB].readByte3() << 24;
      crgb |= C1(mac1);
      crgb |= C2(mac2) << 8;
      crgb |= C3(mac3) << 16;

      data_reg[RGB2].write(crgb);
}

void gte::SQR()
{
      if (shiftFraction())
      {
            /***** INPUTS *****/
            fixedPoint ir1(data_reg[IR1].readLowerHalfword(), 12);
            fixedPoint ir2(data_reg[IR2].readLowerHalfword(), 12);
            fixedPoint ir3(data_reg[IR3].readLowerHalfword(), 12);

            /***** CALCULATION *****/
            fixedPoint mac1 = ir1 * ir1;
            fixedPoint mac2 = ir2 * ir2;
            fixedPoint mac3 = ir3 * ir3;

            /***** OUTPUTS *****/
            data_reg[MAC1].write(A1(mac1, 12));
            data_reg[MAC2].write(A2(mac2, 12));
            data_reg[MAC3].write(A3(mac3, 12));
            data_reg[IR1].writeLowerHalfword(B1(mac1, 12));
            data_reg[IR2].writeLowerHalfword(B2(mac2, 12));
            data_reg[IR3].writeLowerHalfword(B3(mac3, 12));
      }

      else
      {
            /***** INPUTS *****/
            fixedPoint ir1(data_reg[IR1].readLowerHalfword(), 0);
            fixedPoint ir2(data_reg[IR2].readLowerHalfword(), 0);
            fixedPoint ir3(data_reg[IR3].readLowerHalfword(), 0);
            
            /***** CALCULATION *****/
            fixedPoint mac1 = ir1 * ir1;
            fixedPoint mac2 = ir2 * ir2;
            fixedPoint mac3 = ir3 * ir3;

            /***** OUTPUTS *****/
            data_reg[MAC1].write(A1(mac1, 0));
            data_reg[MAC2].write(A2(mac2, 0));
            data_reg[MAC3].write(A3(mac3, 0));
            data_reg[IR1].writeLowerHalfword(B1(mac1, 0));
            data_reg[IR2].writeLowerHalfword(B2(mac2, 0));
            data_reg[IR3].writeLowerHalfword(B3(mac3, 0));
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
      /***** INPUTS *****/
      fixedPoint lr1(control_reg[LR1LR2].readLowerHalfword(), 12);
      fixedPoint lr2(control_reg[LR1LR2].readUpperHalfword(), 12);
      fixedPoint lr3(control_reg[LR3LG1].readLowerHalfword(), 12);
      fixedPoint lg1(control_reg[LR3LG1].readUpperHalfword(), 12);
      fixedPoint lg2(control_reg[LG2LG3].readLowerHalfword(), 12);
      fixedPoint lg3(control_reg[LG2LG3].readUpperHalfword(), 12);
      fixedPoint lb1(control_reg[LB1LB2].readLowerHalfword(), 12);
      fixedPoint lb2(control_reg[LB1LB2].readUpperHalfword(), 12);
      fixedPoint lb3(control_reg[LB3].readLowerHalfword(), 12);

      fixedPoint ir1(data_reg[IR1].readLowerHalfword(), 12);
      fixedPoint ir2(data_reg[IR2].readLowerHalfword(), 12);
      fixedPoint ir3(data_reg[IR3].readLowerHalfword(), 12);

      fixedPoint rbk(control_reg[RBK].read(), 12);
      fixedPoint gbk(control_reg[GBK].read(), 12);
      fixedPoint bbk(control_reg[BBK].read(), 12);

      fixedPoint r(data_reg[RGB].readByte0(), 0, false);
      fixedPoint g(data_reg[RGB].readByte1(), 0, false);
      fixedPoint b(data_reg[RGB].readByte2(), 0, false);

      /***** CALCULATION *****/
      fixedPoint mac1 = rbk + (lr1 * ir1) + (lr2 * ir2) + (lr3 * ir3);
      fixedPoint mac2 = gbk + (lg1 * ir1) + (lg2 * ir2) + (lg3 * ir3);
      fixedPoint mac3 = bbk + (lb1 * ir1) + (lb2 * ir2) + (lb3 * ir3);

      A1(mac1, 12);
      A2(mac2, 12);
      A3(mac3, 12);
      B1(mac1, 12);
      B2(mac2, 12);
      B3(mac3, 12);

      mac1 = r * mac1;
      mac2 = g * mac2;
      mac3 = b * mac3;

      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, 4));
      data_reg[MAC2].write(A2(mac2, 4));
      data_reg[MAC3].write(A3(mac3, 4));
      data_reg[IR1].writeLowerHalfword(B1(mac1, 4));
      data_reg[IR2].writeLowerHalfword(B2(mac2, 4));
      data_reg[IR3].writeLowerHalfword(B3(mac3, 4));

      word crgb = data_reg[RGB].readByte3() << 24;
      crgb |= C1(mac1);
      crgb |= C2(mac2) << 8;
      crgb |= C3(mac3) << 16;

      data_reg[RGB2].write(crgb);
}

void gte::NCLIP()
{
      /***** INPUTS *****/
      fixedPoint sx0(data_reg[SXY0].readLowerHalfword(), 0);
      fixedPoint sx1(data_reg[SXY1].readLowerHalfword(), 0);
      fixedPoint sx2(data_reg[SXY2].readLowerHalfword(), 0);
      fixedPoint sy0(data_reg[SXY0].readUpperHalfword(), 0);
      fixedPoint sy1(data_reg[SXY1].readUpperHalfword(), 0);
      fixedPoint sy2(data_reg[SXY2].readUpperHalfword(), 0);
      
      /***** OUTPUT *****/
      fixedPoint mac0 = (sx0 * sy1) + (sx1 * sy2) + (sx2 * sy0)
                      - (sx0 * sy2) - (sx1 * sy0) - (sx2 * sy1);
      data_reg[MAC0].write(F(mac0));
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
