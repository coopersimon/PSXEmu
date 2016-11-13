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
word gte::A1(fixedPoint &input, unsigned fraction)
{
      bool set_bit = input.checkOverflow(43);
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
      bool set_bit = input.checkOverflow(43);
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
      bool set_bit = input.checkOverflow(43);
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

// these should take in a word. needs lots of work.
word gte::B1(fixedPoint input, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 24, 1);
      }
      return input.getAsWord(16-fraction, fraction);
}

word gte::B2(fixedPoint input, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 23, 1);
      }
      return input.getAsWord(16-fraction, fraction);
}

word gte::B3(fixedPoint input, unsigned fraction)
{
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 22, 1);
      }
      return input.getAsWord(16-fraction, fraction);
}

fixedPoint gte::AB1(fixedPoint input)
{
      input.truncateFraction(12);
      // A
      bool set_bit = input.checkOverflow(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 27, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 30, 1);
      }
      // B
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 24, 1);
      }
      return input;
}

fixedPoint gte::AB2(fixedPoint input)
{
      input.truncateFraction(12);
      // A
      bool set_bit = input.checkOverflow(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 26, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 29, 1);
      }
      // B
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 23, 1);
      }
      return input;
}

fixedPoint gte::AB3(fixedPoint input)
{
      input.truncateFraction(12);
      // A
      bool set_bit = input.checkOverflow(43);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 25, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 28, 1);
      }
      // B
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 22, 1);
      }
      return input;
}

fixedPoint gte::fpB1(fixedPoint input)
{
      // TODO: really not sure about this truncation here.
      input.truncateFraction(4);
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 24, 1);
      }
      return input;
}

fixedPoint gte::fpB2(fixedPoint input)
{
      input.truncateFraction(4);
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 23, 1);
      }
      return input;
}

fixedPoint gte::fpB3(fixedPoint input)
{
      input.truncateFraction(4);
      if (input.checkSaturation(0x7FFF, lm()))
      {
            control_reg[FLAG].writeBits(1, 22, 1);
      }
      return input;
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

word gte::D(fixedPoint &input)
{
      if (input.checkSaturation(0xFFFF, true))
      {
            control_reg[FLAG].writeBits(1, 18, 1);
      }
      return input.getAsWord(16, 0);
}

/*fixedPoint gte::E(fixedPoint &input)
{
      return input;
}*/

word gte::F(fixedPoint &input)
{
      bool set_bit = input.checkOverflow(31);
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

word gte::G1F(fixedPoint &input)
{
      bool set_bit = input.checkOverflow(31);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 15, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 16, 1);
      }

      if (input.checkSaturation(0x3FF, false))
      {
            control_reg[FLAG].writeBits(1, 14, 1);
      }
      return input.getAsWord(16, 0);
}

word gte::G2F(fixedPoint &input)
{
      bool set_bit = input.checkOverflow(31);
      if (set_bit && input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 15, 1);
      }
      else if (set_bit && !input.checkSign())
      {
           control_reg[FLAG].writeBits(1, 16, 1);
      }

      if (input.checkSaturation(0x3FF, false))
      {
            control_reg[FLAG].writeBits(1, 13, 1);
      }
      return input.getAsWord(16, 0);
}

word gte::Hx(fixedPoint &input)
{
      if (input.checkSaturation(0x1000, true))
      {
            control_reg[FLAG].writeBits(1, 12, 1);
      }
      return input.getAsWord(16, 0);
}


/***** INSTRUCTIONS *****/

void gte::RESERVED()
{
      return;
}


void gte::RTPS()
{
      /***** INPUTS *****/
      fixedPoint vx0(data_reg[VXY0].readLowerHalfword(), 0);
      fixedPoint vy0(data_reg[VXY0].readUpperHalfword(), 0);
      fixedPoint vz0(data_reg[VZ0].readLowerHalfword(), 0);

      fixedPoint r11(control_reg[R11R12].readLowerHalfword(), 12);
      fixedPoint r12(control_reg[R11R12].readUpperHalfword(), 12);
      fixedPoint r13(control_reg[R13R21].readLowerHalfword(), 12);
      fixedPoint r21(control_reg[R13R21].readUpperHalfword(), 12);
      fixedPoint r22(control_reg[R22R23].readLowerHalfword(), 12);
      fixedPoint r23(control_reg[R22R23].readUpperHalfword(), 12);
      fixedPoint r31(control_reg[R31R32].readLowerHalfword(), 12);
      fixedPoint r32(control_reg[R31R32].readUpperHalfword(), 12);
      fixedPoint r33(control_reg[R33].readLowerHalfword(), 12);

      fixedPoint trX(control_reg[TRX].read(), 0);
      fixedPoint trY(control_reg[TRY].read(), 0);
      fixedPoint trZ(control_reg[TRZ].read(), 0);

      fixedPoint h(control_reg[H].readLowerHalfword(), 0);
      
      fixedPoint dqa(control_reg[DQA].readLowerHalfword(), 8);
      fixedPoint dqb(control_reg[DQB].readLowerHalfword(), 8);

      fixedPoint ofx(control_reg[OFX].read(), 16);
      fixedPoint ofy(control_reg[OFY].read(), 16);

      /***** CALCULATION *****/
      fixedPoint mac1 = trX + (r11*vx0) + (r12*vy0) + (r13*vz0);
      fixedPoint mac2 = trY + (r21*vx0) + (r22*vy0) + (r23*vz0);
      fixedPoint mac3 = trZ + (r31*vx0) + (r32*vy0) + (r33*vz0);

      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, 0));
      data_reg[MAC2].write(A2(mac2, 0));
      data_reg[MAC3].write(A3(mac3, 0));
      data_reg[IR1].writeLowerHalfword(B1(mac1, 0));
      data_reg[IR2].writeLowerHalfword(B2(mac2, 0));
      data_reg[IR3].writeLowerHalfword(B3(mac3, 0));
      // TODO: B3 will change mac3, we need it unchanged for D
      data_reg[SZ3].write(D(mac3));

      /***** CALCULATION *****/
      fixedPoint h_sz; //divide: h/mac3

      fixedPoint sx = ofx + (mac1 * h_sz);
      fixedPoint sy = ofy + (mac2 * h_sz);
      fixedPoint mac0 = dqb + (dqa * h_sz);

      /***** OUTPUTS *****/
      data_reg[SXYP].write(0);
      data_reg[SXYP].writeLowerHalfword(G1F(sx));
      data_reg[SXYP].writeUpperHalfword(G2F(sy));

      data_reg[MAC0].write(F(mac0));
      data_reg[IR0].writeLowerHalfword(Hx(mac0));
}

void gte::RTPT()
{
      // lm = 0
}

void gte::MVMVA()
{
      /***** INPUTS *****/
      fixedPoint mx11, mx12, mx13;
      fixedPoint mx21, mx22, mx23;
      fixedPoint mx31, mx32, mx33;

      fixedPoint vx;
      fixedPoint vy;
      fixedPoint vz;

      fixedPoint cvx;
      fixedPoint cvy;
      fixedPoint cvz;

      /***** INPUT CONSTRUCTION *****/
      if (MVMVAMultiplyMatrix() == 0)
      {
            // rotation matrix
            mx11 = fixedPoint(control_reg[R11R12].readLowerHalfword(), 12);
            mx12 = fixedPoint(control_reg[R11R12].readUpperHalfword(), 12);
            mx13 = fixedPoint(control_reg[R13R21].readLowerHalfword(), 12);
            mx21 = fixedPoint(control_reg[R13R21].readUpperHalfword(), 12);
            mx22 = fixedPoint(control_reg[R22R23].readLowerHalfword(), 12);
            mx23 = fixedPoint(control_reg[R22R23].readUpperHalfword(), 12);
            mx31 = fixedPoint(control_reg[R31R32].readLowerHalfword(), 12);
            mx32 = fixedPoint(control_reg[R31R32].readUpperHalfword(), 12);
            mx33 = fixedPoint(control_reg[R33].readLowerHalfword(), 12);
      }
      else if (MVMVAMultiplyMatrix() == 1)
      {
            // light matrix
            mx11 = fixedPoint(control_reg[L11L12].readLowerHalfword(), 12);
            mx12 = fixedPoint(control_reg[L11L12].readUpperHalfword(), 12);
            mx13 = fixedPoint(control_reg[L13L21].readLowerHalfword(), 12);
            mx21 = fixedPoint(control_reg[L13L21].readUpperHalfword(), 12);
            mx22 = fixedPoint(control_reg[L22L23].readLowerHalfword(), 12);
            mx23 = fixedPoint(control_reg[L22L23].readUpperHalfword(), 12);
            mx31 = fixedPoint(control_reg[L31L32].readLowerHalfword(), 12);
            mx32 = fixedPoint(control_reg[L31L32].readUpperHalfword(), 12);
            mx33 = fixedPoint(control_reg[L33].readLowerHalfword(), 12);
      }
      else if (MVMVAMultiplyMatrix() == 2)
      {
            // color matrix
            mx11 = fixedPoint(control_reg[LR1LR2].readLowerHalfword(), 12);
            mx12 = fixedPoint(control_reg[LR1LR2].readUpperHalfword(), 12);
            mx13 = fixedPoint(control_reg[LR3LG1].readLowerHalfword(), 12);
            mx21 = fixedPoint(control_reg[LR3LG1].readUpperHalfword(), 12);
            mx22 = fixedPoint(control_reg[LG2LG3].readLowerHalfword(), 12);
            mx23 = fixedPoint(control_reg[LG2LG3].readUpperHalfword(), 12);
            mx31 = fixedPoint(control_reg[LB1LB2].readLowerHalfword(), 12);
            mx32 = fixedPoint(control_reg[LB1LB2].readUpperHalfword(), 12);
            mx33 = fixedPoint(control_reg[LB3].readLowerHalfword(), 12);
      }

      if (MVMVAMultiplyVector() == 0)
      {
            // v0
            vx = fixedPoint(data_reg[VXY0].readLowerHalfword(), 12);
            vy = fixedPoint(data_reg[VXY0].readUpperHalfword(), 12);
            vz = fixedPoint(data_reg[VZ0].readLowerHalfword(), 12);
      }
      else if (MVMVAMultiplyVector() == 1)
      {
            // v1
            vx = fixedPoint(data_reg[VXY1].readLowerHalfword(), 12);
            vy = fixedPoint(data_reg[VXY1].readUpperHalfword(), 12);
            vz = fixedPoint(data_reg[VZ1].readLowerHalfword(), 12);
      }
      else if (MVMVAMultiplyVector() == 2)
      {
            // v2
            vx = fixedPoint(data_reg[VXY2].readLowerHalfword(), 12);
            vy = fixedPoint(data_reg[VXY2].readUpperHalfword(), 12);
            vz = fixedPoint(data_reg[VZ2].readLowerHalfword(), 12);
      }
      else
      {
            // ir
            vx = fixedPoint(data_reg[IR1].readLowerHalfword(), 12);
            vy = fixedPoint(data_reg[IR2].readLowerHalfword(), 12);
            vz = fixedPoint(data_reg[IR3].readLowerHalfword(), 12);
      }

      if (MVMVATranslationVector() == 0)
      {
            // tr
            cvx = fixedPoint(control_reg[TRX].read(), 0);
            cvy = fixedPoint(control_reg[TRY].read(), 0);
            cvz = fixedPoint(control_reg[TRZ].read(), 0);
      }
      else if (MVMVATranslationVector() == 1)
      {
            // bk
            cvx = fixedPoint(control_reg[RBK].read(), 12);
            cvy = fixedPoint(control_reg[GBK].read(), 12);
            cvz = fixedPoint(control_reg[BBK].read(), 12);
      }

      /***** CALCULATION *****/
      fixedPoint mac1 = cvx + (mx11 * vx) + (mx12 * vy) + (mx13 * vz);
      fixedPoint mac2 = cvy + (mx21 * vx) + (mx22 * vy) + (mx23 * vz);
      fixedPoint mac3 = cvz + (mx31 * vx) + (mx32 * vy) + (mx33 * vz);

      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, 4));
      data_reg[MAC2].write(A2(mac2, 4));
      data_reg[MAC3].write(A3(mac3, 4));
      data_reg[IR1].writeLowerHalfword(B1(mac1, 4));
      data_reg[IR2].writeLowerHalfword(B2(mac2, 4));
      data_reg[IR3].writeLowerHalfword(B3(mac3, 4));
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
      fixedPoint mid = rfc - rir; // mid: 27.4
      fixedPoint mac1 = rir + (ir0 * fpB1(mid));

      fixedPoint gir = g * ir2;
      mid = gfc - gir;
      fixedPoint mac2 = gir + (ir0 * fpB2(mid));

      fixedPoint bir = b * ir3;
      mid = bfc - bir;
      fixedPoint mac3 = bir + (ir0 * fpB3(mid));

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
      /***** INPUTS *****/
      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), 12);

      fixedPoint rfc(control_reg[RFC].read(), 4);
      fixedPoint gfc(control_reg[GFC].read(), 4);
      fixedPoint bfc(control_reg[BFC].read(), 4);
      
      fixedPoint r(data_reg[RGB].readByte0(), 0, false);
      fixedPoint g(data_reg[RGB].readByte1(), 0, false);
      fixedPoint b(data_reg[RGB].readByte2(), 0, false);
      
      /***** CALCULATION *****/
      fixedPoint mid = rfc - r;
      fixedPoint mac1 = r + (ir0 * fpB1(mid));

      mid = gfc - g;
      fixedPoint mac2 = g + (ir0 * fpB2(mid));

      mid = bfc - b;
      fixedPoint mac3 = b + (ir0 * fpB3(mid));

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

void gte::DPCT()
{
      /***** INPUTS *****/
      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), 12);

      fixedPoint rfc(control_reg[RFC].read(), 4);
      fixedPoint gfc(control_reg[GFC].read(), 4);
      fixedPoint bfc(control_reg[BFC].read(), 4);
      
      fixedPoint r;
      fixedPoint g;
      fixedPoint b;

      for (unsigned i = 0; i < 3; ++i)
      {
            /***** INPUT CONSTRUCTION *****/
            r = fixedPoint(data_reg[RGB0].readByte0(), 0, false);
            g = fixedPoint(data_reg[RGB0].readByte1(), 0, false);
            b = fixedPoint(data_reg[RGB0].readByte2(), 0, false);
            
            /***** CALCULATION *****/
            fixedPoint mid = rfc - r;
            fixedPoint mac1 = r + (ir0 * fpB1(mid));

            mid = gfc - g;
            fixedPoint mac2 = g + (ir0 * fpB2(mid));

            mid = bfc - b;
            fixedPoint mac3 = b + (ir0 * fpB3(mid));

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
      fixedPoint mid = rfc - ir1; // TODO investigate subtraction & truncation
      fixedPoint mac1 = ir1 + (ir0 * fpB1(mid));

      mid = gfc - ir2;
      fixedPoint mac2 = ir2 + (ir0 * fpB2(mid));

      mid = bfc - ir3;
      fixedPoint mac3 = ir3 + (ir0 * fpB3(mid));
      
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
      unsigned fraction;
      if (shiftFraction())
            fraction = 12;
      else
            fraction = 0;
      
      /***** INPUTS *****/
      fixedPoint ir1(data_reg[IR1].readLowerHalfword(), fraction);
      fixedPoint ir2(data_reg[IR2].readLowerHalfword(), fraction);
      fixedPoint ir3(data_reg[IR3].readLowerHalfword(), fraction);

      /***** CALCULATION *****/
      fixedPoint mac1 = ir1 * ir1;
      fixedPoint mac2 = ir2 * ir2;
      fixedPoint mac3 = ir3 * ir3;

      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, fraction));
      data_reg[MAC2].write(A2(mac2, fraction));
      data_reg[MAC3].write(A3(mac3, fraction));
      data_reg[IR1].writeLowerHalfword(B1(mac1, fraction));
      data_reg[IR2].writeLowerHalfword(B2(mac2, fraction));
      data_reg[IR3].writeLowerHalfword(B3(mac3, fraction));
}

void gte::NCS()
{
      /***** INPUTS *****/
      fixedPoint l11(control_reg[L11L12].readLowerHalfword(), 12);
      fixedPoint l12(control_reg[L11L12].readUpperHalfword(), 12);
      fixedPoint l13(control_reg[L13L21].readLowerHalfword(), 12);
      fixedPoint l21(control_reg[L13L21].readUpperHalfword(), 12);
      fixedPoint l22(control_reg[L22L23].readLowerHalfword(), 12);
      fixedPoint l23(control_reg[L22L23].readUpperHalfword(), 12);
      fixedPoint l31(control_reg[L31L32].readLowerHalfword(), 12);
      fixedPoint l32(control_reg[L31L32].readUpperHalfword(), 12);
      fixedPoint l33(control_reg[L33].readLowerHalfword(), 12);

      fixedPoint vx0(data_reg[VXY0].readLowerHalfword(), 12);
      fixedPoint vy0(data_reg[VXY0].readUpperHalfword(), 12);
      fixedPoint vz0(data_reg[VZ0].readLowerHalfword(), 12);

      fixedPoint lr1(control_reg[LR1LR2].readLowerHalfword(), 12);
      fixedPoint lr2(control_reg[LR1LR2].readUpperHalfword(), 12);
      fixedPoint lr3(control_reg[LR3LG1].readLowerHalfword(), 12);
      fixedPoint lg1(control_reg[LR3LG1].readUpperHalfword(), 12);
      fixedPoint lg2(control_reg[LG2LG3].readLowerHalfword(), 12);
      fixedPoint lg3(control_reg[LG2LG3].readUpperHalfword(), 12);
      fixedPoint lb1(control_reg[LB1LB2].readLowerHalfword(), 12);
      fixedPoint lb2(control_reg[LB1LB2].readUpperHalfword(), 12);
      fixedPoint lb3(control_reg[LB3].readLowerHalfword(), 12);

      fixedPoint rbk(control_reg[RBK].read(), 12);
      fixedPoint gbk(control_reg[GBK].read(), 12);
      fixedPoint bbk(control_reg[BBK].read(), 12);

      /***** CALCULATION *****/
      fixedPoint mid_1 = AB1((l11 * vx0) + (l12 * vy0) + (l13 * vz0));
      fixedPoint mid_2 = AB2((l21 * vx0) + (l22 * vy0) + (l23 * vz0));
      fixedPoint mid_3 = AB3((l31 * vx0) + (l32 * vy0) + (l33 * vz0));

      fixedPoint mac1 = rbk + (lr1 * mid_1) + (lr2 * mid_2) + (lr3 * mid_3);
      fixedPoint mac2 = gbk + (lg1 * mid_1) + (lg2 * mid_2) + (lg3 * mid_3);
      fixedPoint mac3 = bbk + (lb1 * mid_1) + (lb2 * mid_2) + (lb3 * mid_3);

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

void gte::NCT()
{
      /***** INPUTS *****/
      fixedPoint l11(control_reg[L11L12].readLowerHalfword(), 12);
      fixedPoint l12(control_reg[L11L12].readUpperHalfword(), 12);
      fixedPoint l13(control_reg[L13L21].readLowerHalfword(), 12);
      fixedPoint l21(control_reg[L13L21].readUpperHalfword(), 12);
      fixedPoint l22(control_reg[L22L23].readLowerHalfword(), 12);
      fixedPoint l23(control_reg[L22L23].readUpperHalfword(), 12);
      fixedPoint l31(control_reg[L31L32].readLowerHalfword(), 12);
      fixedPoint l32(control_reg[L31L32].readUpperHalfword(), 12);
      fixedPoint l33(control_reg[L33].readLowerHalfword(), 12);

      fixedPoint vx;
      fixedPoint vy;
      fixedPoint vz;

      fixedPoint lr1(control_reg[LR1LR2].readLowerHalfword(), 12);
      fixedPoint lr2(control_reg[LR1LR2].readUpperHalfword(), 12);
      fixedPoint lr3(control_reg[LR3LG1].readLowerHalfword(), 12);
      fixedPoint lg1(control_reg[LR3LG1].readUpperHalfword(), 12);
      fixedPoint lg2(control_reg[LG2LG3].readLowerHalfword(), 12);
      fixedPoint lg3(control_reg[LG2LG3].readUpperHalfword(), 12);
      fixedPoint lb1(control_reg[LB1LB2].readLowerHalfword(), 12);
      fixedPoint lb2(control_reg[LB1LB2].readUpperHalfword(), 12);
      fixedPoint lb3(control_reg[LB3].readLowerHalfword(), 12);

      fixedPoint rbk(control_reg[RBK].read(), 12);
      fixedPoint gbk(control_reg[GBK].read(), 12);
      fixedPoint bbk(control_reg[BBK].read(), 12);

      for (unsigned i = 0; i < 3; ++i)
      {
            /***** INPUT CONSTRUCTION *****/
            if (i == 0)
            {
                  vx = fixedPoint(data_reg[VXY0].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY0].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ0].readLowerHalfword(), 12);
            }
            else if (i == 1)
            {
                  vx = fixedPoint(data_reg[VXY1].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY1].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ1].readLowerHalfword(), 12);
            }
            else if (i == 2)
            {
                  vx = fixedPoint(data_reg[VXY2].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY2].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ2].readLowerHalfword(), 12);
            }

            /***** CALCULATION *****/
            fixedPoint mid_1 = AB1((l11 * vx) + (l12 * vy) + (l13 * vz));
            fixedPoint mid_2 = AB2((l21 * vx) + (l22 * vy) + (l23 * vz));
            fixedPoint mid_3 = AB3((l31 * vx) + (l32 * vy) + (l33 * vz));

            fixedPoint mac1 = rbk + (lr1 * mid_1) + (lr2 * mid_2) + (lr3 * mid_3);
            fixedPoint mac2 = gbk + (lg1 * mid_1) + (lg2 * mid_2) + (lg3 * mid_3);
            fixedPoint mac3 = bbk + (lb1 * mid_1) + (lb2 * mid_2) + (lb3 * mid_3);
      
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
}

void gte::NCDS()
{
      /***** INPUTS *****/
      fixedPoint l11(control_reg[L11L12].readLowerHalfword(), 12);
      fixedPoint l12(control_reg[L11L12].readUpperHalfword(), 12);
      fixedPoint l13(control_reg[L13L21].readLowerHalfword(), 12);
      fixedPoint l21(control_reg[L13L21].readUpperHalfword(), 12);
      fixedPoint l22(control_reg[L22L23].readLowerHalfword(), 12);
      fixedPoint l23(control_reg[L22L23].readUpperHalfword(), 12);
      fixedPoint l31(control_reg[L31L32].readLowerHalfword(), 12);
      fixedPoint l32(control_reg[L31L32].readUpperHalfword(), 12);
      fixedPoint l33(control_reg[L33].readLowerHalfword(), 12);

      fixedPoint vx0(data_reg[VXY0].readLowerHalfword(), 12);
      fixedPoint vy0(data_reg[VXY0].readUpperHalfword(), 12);
      fixedPoint vz0(data_reg[VZ0].readLowerHalfword(), 12);

      fixedPoint lr1(control_reg[LR1LR2].readLowerHalfword(), 12);
      fixedPoint lr2(control_reg[LR1LR2].readUpperHalfword(), 12);
      fixedPoint lr3(control_reg[LR3LG1].readLowerHalfword(), 12);
      fixedPoint lg1(control_reg[LR3LG1].readUpperHalfword(), 12);
      fixedPoint lg2(control_reg[LG2LG3].readLowerHalfword(), 12);
      fixedPoint lg3(control_reg[LG2LG3].readUpperHalfword(), 12);
      fixedPoint lb1(control_reg[LB1LB2].readLowerHalfword(), 12);
      fixedPoint lb2(control_reg[LB1LB2].readUpperHalfword(), 12);
      fixedPoint lb3(control_reg[LB3].readLowerHalfword(), 12);

      fixedPoint rbk(control_reg[RBK].read(), 12);
      fixedPoint gbk(control_reg[GBK].read(), 12);
      fixedPoint bbk(control_reg[BBK].read(), 12);

      fixedPoint rfc(control_reg[RFC].read(), 4);
      fixedPoint gfc(control_reg[GFC].read(), 4);
      fixedPoint bfc(control_reg[BFC].read(), 4);
      
      fixedPoint r(data_reg[RGB0].readByte0(), 0, false);
      fixedPoint g(data_reg[RGB0].readByte1(), 0, false);
      fixedPoint b(data_reg[RGB0].readByte2(), 0, false);

      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), 12);

      /***** CALCULATION *****/
      fixedPoint mid_a1 = AB1((l11 * vx0) + (l12 * vy0) + (l13 * vz0));
      fixedPoint mid_a2 = AB2((l21 * vx0) + (l22 * vy0) + (l23 * vz0));
      fixedPoint mid_a3 = AB3((l31 * vx0) + (l32 * vy0) + (l33 * vz0));

      fixedPoint mid_b1 = AB1(rbk + (lr1 * mid_a1) + (lr2 * mid_a2) + (lr3 * mid_a3));
      fixedPoint mid_b2 = AB2(gbk + (lg1 * mid_a1) + (lg2 * mid_a2) + (lg3 * mid_a3));
      fixedPoint mid_b3 = AB3(bbk + (lb1 * mid_a1) + (lb2 * mid_a2) + (lb3 * mid_a3));

      fixedPoint rir = r * mid_b1;
      //rir.truncateFraction(4);
      fixedPoint mid = rfc - rir;
      fixedPoint mac1 = rir + (ir0 * fpB1(mid));

      fixedPoint gir = g * mid_b2;
      //gir.truncateFraction(4);
      mid = gfc - gir;
      fixedPoint mac2 = gir + (ir0 * fpB2(mid));

      fixedPoint bir = b * mid_b3;
      //bir.truncateFraction(4);
      mid = bfc - bir;
      fixedPoint mac3 = bir + (ir0 * fpB3(mid));

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

void gte::NCDT()
{
      /***** INPUTS *****/
      fixedPoint l11(control_reg[L11L12].readLowerHalfword(), 12);
      fixedPoint l12(control_reg[L11L12].readUpperHalfword(), 12);
      fixedPoint l13(control_reg[L13L21].readLowerHalfword(), 12);
      fixedPoint l21(control_reg[L13L21].readUpperHalfword(), 12);
      fixedPoint l22(control_reg[L22L23].readLowerHalfword(), 12);
      fixedPoint l23(control_reg[L22L23].readUpperHalfword(), 12);
      fixedPoint l31(control_reg[L31L32].readLowerHalfword(), 12);
      fixedPoint l32(control_reg[L31L32].readUpperHalfword(), 12);
      fixedPoint l33(control_reg[L33].readLowerHalfword(), 12);

      fixedPoint vx;
      fixedPoint vy;
      fixedPoint vz;

      fixedPoint lr1(control_reg[LR1LR2].readLowerHalfword(), 12);
      fixedPoint lr2(control_reg[LR1LR2].readUpperHalfword(), 12);
      fixedPoint lr3(control_reg[LR3LG1].readLowerHalfword(), 12);
      fixedPoint lg1(control_reg[LR3LG1].readUpperHalfword(), 12);
      fixedPoint lg2(control_reg[LG2LG3].readLowerHalfword(), 12);
      fixedPoint lg3(control_reg[LG2LG3].readUpperHalfword(), 12);
      fixedPoint lb1(control_reg[LB1LB2].readLowerHalfword(), 12);
      fixedPoint lb2(control_reg[LB1LB2].readUpperHalfword(), 12);
      fixedPoint lb3(control_reg[LB3].readLowerHalfword(), 12);

      fixedPoint rbk(control_reg[RBK].read(), 12);
      fixedPoint gbk(control_reg[GBK].read(), 12);
      fixedPoint bbk(control_reg[BBK].read(), 12);

      fixedPoint rfc(control_reg[RFC].read(), 4);
      fixedPoint gfc(control_reg[GFC].read(), 4);
      fixedPoint bfc(control_reg[BFC].read(), 4);
      
      fixedPoint r(data_reg[RGB0].readByte0(), 0, false);
      fixedPoint g(data_reg[RGB0].readByte1(), 0, false);
      fixedPoint b(data_reg[RGB0].readByte2(), 0, false);

      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), 12);

      for (unsigned i = 0; i < 3; ++i)
      {
            /***** INPUT CONSTRUCTION *****/
            if (i == 0)
            {
                  vx = fixedPoint(data_reg[VXY0].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY0].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ0].readLowerHalfword(), 12);
            }
            else if (i == 1)
            {
                  vx = fixedPoint(data_reg[VXY1].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY1].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ1].readLowerHalfword(), 12);
            }
            else if (i == 2)
            {
                  vx = fixedPoint(data_reg[VXY2].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY2].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ2].readLowerHalfword(), 12);
            }

            /***** CALCULATION *****/
            fixedPoint mid_a1 = AB1((l11 * vx) + (l12 * vy) + (l13 * vz));
            fixedPoint mid_a2 = AB2((l21 * vx) + (l22 * vy) + (l23 * vz));
            fixedPoint mid_a3 = AB3((l31 * vx) + (l32 * vy) + (l33 * vz));

            fixedPoint mid_b1 = AB1(rbk + (lr1 * mid_a1) + (lr2 * mid_a2) + (lr3 * mid_a3));
            fixedPoint mid_b2 = AB2(gbk + (lg1 * mid_a1) + (lg2 * mid_a2) + (lg3 * mid_a3));
            fixedPoint mid_b3 = AB3(bbk + (lb1 * mid_a1) + (lb2 * mid_a2) + (lb3 * mid_a3));

            fixedPoint rir = r * mid_b1;
            rir.truncateFraction(4);
            fixedPoint mid = rfc - rir;
            fixedPoint mac1 = rir + (ir0 * fpB1(mid));

            fixedPoint gir = g * mid_b2;
            gir.truncateFraction(4);
            mid = gfc - gir;
            fixedPoint mac2 = gir + (ir0 * fpB2(mid));

            fixedPoint bir = b * mid_b3;
            bir.truncateFraction(4);
            mid = bfc - bir;
            fixedPoint mac3 = bir + (ir0 * fpB3(mid));

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
}

void gte::NCCS()
{
      /***** INPUTS *****/
      fixedPoint l11(control_reg[L11L12].readLowerHalfword(), 12);
      fixedPoint l12(control_reg[L11L12].readUpperHalfword(), 12);
      fixedPoint l13(control_reg[L13L21].readLowerHalfword(), 12);
      fixedPoint l21(control_reg[L13L21].readUpperHalfword(), 12);
      fixedPoint l22(control_reg[L22L23].readLowerHalfword(), 12);
      fixedPoint l23(control_reg[L22L23].readUpperHalfword(), 12);
      fixedPoint l31(control_reg[L31L32].readLowerHalfword(), 12);
      fixedPoint l32(control_reg[L31L32].readUpperHalfword(), 12);
      fixedPoint l33(control_reg[L33].readLowerHalfword(), 12);

      fixedPoint vx0(data_reg[VXY0].readLowerHalfword(), 12);
      fixedPoint vy0(data_reg[VXY0].readUpperHalfword(), 12);
      fixedPoint vz0(data_reg[VZ0].readLowerHalfword(), 12);

      fixedPoint lr1(control_reg[LR1LR2].readLowerHalfword(), 12);
      fixedPoint lr2(control_reg[LR1LR2].readUpperHalfword(), 12);
      fixedPoint lr3(control_reg[LR3LG1].readLowerHalfword(), 12);
      fixedPoint lg1(control_reg[LR3LG1].readUpperHalfword(), 12);
      fixedPoint lg2(control_reg[LG2LG3].readLowerHalfword(), 12);
      fixedPoint lg3(control_reg[LG2LG3].readUpperHalfword(), 12);
      fixedPoint lb1(control_reg[LB1LB2].readLowerHalfword(), 12);
      fixedPoint lb2(control_reg[LB1LB2].readUpperHalfword(), 12);
      fixedPoint lb3(control_reg[LB3].readLowerHalfword(), 12);

      fixedPoint rbk(control_reg[RBK].read(), 12);
      fixedPoint gbk(control_reg[GBK].read(), 12);
      fixedPoint bbk(control_reg[BBK].read(), 12);

      fixedPoint r(data_reg[RGB0].readByte0(), 0, false);
      fixedPoint g(data_reg[RGB0].readByte1(), 0, false);
      fixedPoint b(data_reg[RGB0].readByte2(), 0, false);

      /***** CALCULATION *****/
      fixedPoint mid_a1 = AB1((l11 * vx0) + (l12 * vy0) + (l13 * vz0));
      fixedPoint mid_a2 = AB2((l21 * vx0) + (l22 * vy0) + (l23 * vz0));
      fixedPoint mid_a3 = AB3((l31 * vx0) + (l32 * vy0) + (l33 * vz0));

      fixedPoint mid_b1 = AB1(rbk + (lr1 * mid_a1) + (lr2 * mid_a2) + (lr3 * mid_a3));
      fixedPoint mid_b2 = AB2(gbk + (lg1 * mid_a1) + (lg2 * mid_a2) + (lg3 * mid_a3));
      fixedPoint mid_b3 = AB3(bbk + (lb1 * mid_a1) + (lb2 * mid_a2) + (lb3 * mid_a3));

      fixedPoint mac1 = r * mid_b1;
      fixedPoint mac2 = g * mid_b2;
      fixedPoint mac3 = b * mid_b3;

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

void gte::NCCT()
{
      /***** INPUTS *****/
      fixedPoint l11(control_reg[L11L12].readLowerHalfword(), 12);
      fixedPoint l12(control_reg[L11L12].readUpperHalfword(), 12);
      fixedPoint l13(control_reg[L13L21].readLowerHalfword(), 12);
      fixedPoint l21(control_reg[L13L21].readUpperHalfword(), 12);
      fixedPoint l22(control_reg[L22L23].readLowerHalfword(), 12);
      fixedPoint l23(control_reg[L22L23].readUpperHalfword(), 12);
      fixedPoint l31(control_reg[L31L32].readLowerHalfword(), 12);
      fixedPoint l32(control_reg[L31L32].readUpperHalfword(), 12);
      fixedPoint l33(control_reg[L33].readLowerHalfword(), 12);

      fixedPoint vx;
      fixedPoint vy;
      fixedPoint vz;

      fixedPoint lr1(control_reg[LR1LR2].readLowerHalfword(), 12);
      fixedPoint lr2(control_reg[LR1LR2].readUpperHalfword(), 12);
      fixedPoint lr3(control_reg[LR3LG1].readLowerHalfword(), 12);
      fixedPoint lg1(control_reg[LR3LG1].readUpperHalfword(), 12);
      fixedPoint lg2(control_reg[LG2LG3].readLowerHalfword(), 12);
      fixedPoint lg3(control_reg[LG2LG3].readUpperHalfword(), 12);
      fixedPoint lb1(control_reg[LB1LB2].readLowerHalfword(), 12);
      fixedPoint lb2(control_reg[LB1LB2].readUpperHalfword(), 12);
      fixedPoint lb3(control_reg[LB3].readLowerHalfword(), 12);

      fixedPoint rbk(control_reg[RBK].read(), 12);
      fixedPoint gbk(control_reg[GBK].read(), 12);
      fixedPoint bbk(control_reg[BBK].read(), 12);

      fixedPoint r(data_reg[RGB0].readByte0(), 0, false);
      fixedPoint g(data_reg[RGB0].readByte1(), 0, false);
      fixedPoint b(data_reg[RGB0].readByte2(), 0, false);

      for (unsigned i = 0; i < 3; ++i)
      {
            /***** INPUT CONSTRUCTION *****/
            if (i == 0)
            {
                  vx = fixedPoint(data_reg[VXY0].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY0].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ0].readLowerHalfword(), 12);
            }
            else if (i == 1)
            {
                  vx = fixedPoint(data_reg[VXY1].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY1].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ1].readLowerHalfword(), 12);
            }
            else if (i == 2)
            {
                  vx = fixedPoint(data_reg[VXY2].readLowerHalfword(), 12);
                  vy = fixedPoint(data_reg[VXY2].readUpperHalfword(), 12);
                  vz = fixedPoint(data_reg[VZ2].readLowerHalfword(), 12);
            }

            /***** CALCULATION *****/
            fixedPoint mid_a1 = AB1((l11 * vx) + (l12 * vy) + (l13 * vz));
            fixedPoint mid_a2 = AB2((l21 * vx) + (l22 * vy) + (l23 * vz));
            fixedPoint mid_a3 = AB3((l31 * vx) + (l32 * vy) + (l33 * vz));

            fixedPoint mid_b1 = AB1(rbk + (lr1 * mid_a1) + (lr2 * mid_a2) + (lr3 * mid_a3));
            fixedPoint mid_b2 = AB2(gbk + (lg1 * mid_a1) + (lg2 * mid_a2) + (lg3 * mid_a3));
            fixedPoint mid_b3 = AB3(bbk + (lb1 * mid_a1) + (lb2 * mid_a2) + (lb3 * mid_a3));

            fixedPoint mac1 = r * mid_b1;
            fixedPoint mac2 = g * mid_b2;
            fixedPoint mac3 = b * mid_b3;

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
}

void gte::CDP()
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

      fixedPoint rfc(control_reg[RFC].read(), 4);
      fixedPoint gfc(control_reg[GFC].read(), 4);
      fixedPoint bfc(control_reg[BFC].read(), 4);
      
      fixedPoint r(data_reg[RGB].readByte0(), 0, false);
      fixedPoint g(data_reg[RGB].readByte1(), 0, false);
      fixedPoint b(data_reg[RGB].readByte2(), 0, false);

      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), 12);
      
      /***** CALCULATION *****/
      fixedPoint mid_1 = AB1(rbk + (lr1 * ir1) + (lr2 * ir2) + (lr3 * ir3));
      fixedPoint mid_2 = AB2(gbk + (lg1 * ir1) + (lg2 * ir2) + (lg3 * ir3));
      fixedPoint mid_3 = AB3(bbk + (lb1 * ir1) + (lb2 * ir2) + (lb3 * ir3));

      fixedPoint rir = r * mid_1;
      rir.truncateFraction(4);
      fixedPoint mid = rfc - rir;
      fixedPoint mac1 = rir + (ir0 * fpB1(mid));

      fixedPoint gir = g * mid_2;
      gir.truncateFraction(4);
      mid = gfc - gir;
      fixedPoint mac2 = gir + (ir0 * fpB2(mid));

      fixedPoint bir = b * mid_3;
      bir.truncateFraction(4);
      mid = bfc - bir;
      fixedPoint mac3 = bir + (ir0 * fpB3(mid));

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
      fixedPoint mid_1 = AB1(rbk + (lr1 * ir1) + (lr2 * ir2) + (lr3 * ir3));
      fixedPoint mid_2 = AB2(gbk + (lg1 * ir1) + (lg2 * ir2) + (lg3 * ir3));
      fixedPoint mid_3 = AB3(bbk + (lb1 * ir1) + (lb2 * ir2) + (lb3 * ir3));

      fixedPoint mac1 = r * mid_1;
      fixedPoint mac2 = g * mid_2;
      fixedPoint mac3 = b * mid_3;

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
      
      /***** CALCULATION *****/
      fixedPoint mac0 = (sx0 * sy1) + (sx1 * sy2) + (sx2 * sy0)
                      - (sx0 * sy2) - (sx1 * sy0) - (sx2 * sy1);
      
      /***** OUTPUT *****/
      data_reg[MAC0].write(F(mac0));
}

void gte::AVSZ3()
{
      /***** INPUTS *****/
      fixedPoint sz1(data_reg[SZ1].readLowerHalfword(), 0);
      fixedPoint sz2(data_reg[SZ2].readLowerHalfword(), 0);
      fixedPoint sz3(data_reg[SZ3].readLowerHalfword(), 0);
      
      fixedPoint zsf3(control_reg[ZSF3].readLowerHalfword(), 12);

      /***** CALCULATION *****/
      fixedPoint mac0 = (zsf3 * sz1) + (zsf3 * sz2) + (zsf3 * sz3);

      /***** OUTPUTS *****/
      data_reg[MAC0].write(F(mac0));
      data_reg[OTZ].write(D(mac0));
}

void gte::AVSZ4()
{
      /***** INPUTS *****/
      fixedPoint sz0(data_reg[SZ0].readLowerHalfword(), 0);
      fixedPoint sz1(data_reg[SZ1].readLowerHalfword(), 0);
      fixedPoint sz2(data_reg[SZ2].readLowerHalfword(), 0);
      fixedPoint sz3(data_reg[SZ3].readLowerHalfword(), 0);
      
      fixedPoint zsf4(control_reg[ZSF4].readLowerHalfword(), 12);

      /***** CALCULATION *****/
      fixedPoint mac0 = (zsf4 * sz0) + (zsf4 * sz1)
                      + (zsf4 * sz2) + (zsf4 * sz3);

      /***** OUTPUTS *****/
      data_reg[MAC0].write(F(mac0));
      data_reg[OTZ].write(D(mac0));
}

void gte::OP()
{
      unsigned fraction;
      if (shiftFraction())
            fraction = 12;
      else
            fraction = 0;

      /***** INPUTS *****/
      fixedPoint d1(control_reg[R11R12].readLowerHalfword(), fraction);
      fixedPoint d2(control_reg[R22R23].readLowerHalfword(), fraction);
      fixedPoint d3(control_reg[R33].readLowerHalfword(), fraction);

      fixedPoint ir1(data_reg[IR1].readLowerHalfword(), fraction);
      fixedPoint ir2(data_reg[IR2].readLowerHalfword(), fraction);
      fixedPoint ir3(data_reg[IR3].readLowerHalfword(), fraction);

      /***** CALCULATION *****/
      fixedPoint mac1 = (d2 * ir3) - (d3 * ir2);
      fixedPoint mac2 = (d3 * ir1) - (d1 * ir3);
      fixedPoint mac3 = (d1 * ir2) - (d2 * ir1);

      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, fraction));
      data_reg[MAC2].write(A2(mac2, fraction));
      data_reg[MAC3].write(A3(mac3, fraction));
      data_reg[IR1].writeLowerHalfword(B1(mac1, fraction));
      data_reg[IR2].writeLowerHalfword(B2(mac2, fraction));
      data_reg[IR3].writeLowerHalfword(B3(mac3, fraction));
}

void gte::GPF()
{
      unsigned fraction;
      if (shiftFraction())
            fraction = 12;
      else
            fraction = 0;

      /***** INPUTS *****/
      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), fraction);
      fixedPoint ir1(data_reg[IR1].readLowerHalfword(), fraction);
      fixedPoint ir2(data_reg[IR2].readLowerHalfword(), fraction);
      fixedPoint ir3(data_reg[IR3].readLowerHalfword(), fraction);

      /***** CALCULATION *****/
      fixedPoint mac1 = ir0 * ir1;
      fixedPoint mac2 = ir0 * ir2;
      fixedPoint mac3 = ir0 * ir3;

      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, fraction));
      data_reg[MAC2].write(A2(mac2, fraction));
      data_reg[MAC3].write(A3(mac3, fraction));
      data_reg[IR1].writeLowerHalfword(B1(mac1, fraction));
      data_reg[IR2].writeLowerHalfword(B2(mac2, fraction));
      data_reg[IR3].writeLowerHalfword(B3(mac3, fraction));

      word crgb = data_reg[RGB].readByte3() << 24;
      crgb |= C1(mac1);
      crgb |= C2(mac2) << 8;
      crgb |= C3(mac3) << 16;

      data_reg[RGB2].write(crgb);
}

void gte::GPL()
{
      unsigned fraction;
      if (shiftFraction())
            fraction = 12;
      else
            fraction = 0;

      /***** INPUTS *****/
      fixedPoint ir0(data_reg[IR0].readLowerHalfword(), fraction);
      fixedPoint ir1(data_reg[IR1].readLowerHalfword(), fraction);
      fixedPoint ir2(data_reg[IR2].readLowerHalfword(), fraction);
      fixedPoint ir3(data_reg[IR3].readLowerHalfword(), fraction);

      fixedPoint mac1(data_reg[MAC1].read(), 0);
      fixedPoint mac2(data_reg[MAC2].read(), 0);
      fixedPoint mac3(data_reg[MAC3].read(), 0);

      /***** CALCULATION *****/
      mac1 = mac1 + ir0 * ir1;
      mac2 = mac2 + ir0 * ir2;
      mac3 = mac3 + ir0 * ir3;

      /***** OUTPUTS *****/
      data_reg[MAC1].write(A1(mac1, fraction));
      data_reg[MAC2].write(A2(mac2, fraction));
      data_reg[MAC3].write(A3(mac3, fraction));
      data_reg[IR1].writeLowerHalfword(B1(mac1, fraction));
      data_reg[IR2].writeLowerHalfword(B2(mac2, fraction));
      data_reg[IR3].writeLowerHalfword(B3(mac3, fraction));

      word crgb = data_reg[RGB].readByte3() << 24;
      crgb |= C1(mac1);
      crgb |= C2(mac2) << 8;
      crgb |= C3(mac3) << 16;

      data_reg[RGB2].write(crgb);
}
