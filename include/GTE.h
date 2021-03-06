#ifndef GTE_H
#define GTE_H

/*
 *    GTE.h
 * 
 *    Declares the Geometry Transformation Engine class
 *    Coprocessor 2
 */

// includes
#include <Coprocessor.h>
#include <FixedPointMaths.h>
#include <vector>
#include <functional>
#include <list>

namespace test
{
      class GTEtestbench;
};

/***** REGISTERS *****/

// GTERegs are like CoprocessorRegs with some extra access functions
class GTEReg : public CoprocessorReg
{
public:
      GTEReg() : CoprocessorReg() {}

      inline byte readByte0() const { return data & 0xFF; }
      inline byte readByte1() const { return (data >> 8) & 0xFF; }
      inline byte readByte2() const { return (data >> 16) & 0xFF; }
      inline byte readByte3() const { return (data >> 24) & 0xFF; }

      inline void writeByte0(word in) { data &= 0xFFFFFF00; data |= in & 0x000000FF; }
      inline void writeByte1(word in) { data &= 0xFFFF00FF; data |= (in << 8) & 0x0000FF00; }
      inline void writeByte2(word in) { data &= 0xFF00FFFF; data |= (in << 16) & 0x00FF0000; }
      inline void writeByte3(word in) { data &= 0x00FFFFFF; data |= (in << 24) & 0xFF000000; }

      inline halfword readLowerHalfword() const { return data & 0xFFFF; }
      inline halfword readUpperHalfword() const { return (data >> 16) & 0xFFFF; }

      inline void writeLowerHalfword(word in) { data &= 0xFFFF0000; data |= in & 0x0000FFFF; }
      inline void writeUpperHalfword(word in) { data &= 0x0000FFFF; data |= (in << 16) & 0xFFFF0000; }
};

// GTEDataRegs are like GTERegs with FIFO chains
class GTEDataReg : public GTEReg
{
      std::list<GTEDataReg*> FIFO_chain;
      GTEDataReg *mirror;

public:
      GTEDataReg() : GTEReg(), mirror(NULL) {}

      void addFIFOReg(GTEDataReg* in) { FIFO_chain.push_back(in); }

      void addFIFOMirror(GTEDataReg* in) { mirror = in; }

      inline void write(word in)
      {
            if (FIFO_chain.empty())
            {
                  data = in;
            }
            else
            {
                  word previous_data = data;
                  for (auto i : FIFO_chain)
                  {
                        data = i->read();
                        i->write(previous_data);
                        previous_data = data;
                  }

                  if (mirror)
                        mirror->write(in);

                  data = in;
            }
      } 
};


// gte: cop2, deals with vector and matrix transformations
class gte : public coprocessor
{
      /*** REGISTERS ***/
      GTEReg control_reg[32];
	GTEDataReg data_reg[32];

      /*** FUNCTION POINTERS ***/
      std::vector<std::function<void(gte*)>> opcodes;

      /*** INSTRUCTION ***/
      unsigned instruction;

public:
      // TODO: constructor, register instructions
      gte();

	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg) override;
	word readDataReg(unsigned source_reg) const override;
	void writeControlReg(word data_in, unsigned dest_reg) override;
	word readControlReg(unsigned source_reg) const override;

      // decode and run instruction
      void executeInstruction(unsigned instruction) override;

private:
      /*** INLINE FUNCTIONS ***/
      // decoding instruction
      inline bool shiftFraction() const { return (instruction >> 19) & 1; }
      inline unsigned MVMVAMultiplyMatrix() const { return (instruction >> 17) & 3; }
      inline unsigned MVMVAMultiplyVector() const { return (instruction >> 15) & 3; }
      inline unsigned MVMVATranslationVector() const { return (instruction >> 13) & 3; }
      inline bool lm() const { return (instruction >> 10) & 1; }
      inline unsigned GTECommand() const { return instruction & 0x3F; }

      /*** FLAG BITS ***/
      // Set bits in FLAG register & return word

      // for MAC return: check 43 bit overflow & truncate
      word A1(fixedPoint &input, unsigned fraction);
      word A2(fixedPoint &input, unsigned fraction);
      word A3(fixedPoint &input, unsigned fraction);

      // for IR return: saturate between -8000/0,7FFF
      word B1(fixedPoint input, unsigned fraction);
      word B2(fixedPoint input, unsigned fraction);
      word B3(fixedPoint input, unsigned fraction);

      // same as A and B, for mid-calculation
      fixedPoint AB1(fixedPoint input);
      fixedPoint AB2(fixedPoint input);
      fixedPoint AB3(fixedPoint input);

      // same as B, for mid-calculation
      fixedPoint fpB1(fixedPoint input);
      fixedPoint fpB2(fixedPoint input);
      fixedPoint fpB3(fixedPoint input);

      // for RGB return: saturate between 0,FF
      word C1(fixedPoint &input);
      word C2(fixedPoint &input);
      word C3(fixedPoint &input);

      // SZ3 or OTZ return: saturate between 0,FFFF
      word D(fixedPoint &input);

      // divide overflow
      //word E(fixedPoint &input);
      
      // for MAC0 return: check 31 bit overflow & truncate
      word F(fixedPoint &input);
      
      // for SXY return: saturate between -400,3FF & check 31 bit overflow
      word G1F(fixedPoint &input);
      word G2F(fixedPoint &input);

      // for IR0 return: saturate between 0,1000
      word Hx(fixedPoint &input);


	/*** INSTRUCTIONS ***/
      void RESERVED();

      // perspective transform
      void RTPS();      //TODO
      // perspective transform (x3)
      void RTPT();      //TODO
      // multiply vector by matrix and vector addition
      void MVMVA();
      // depth cue colour light
      void DCPL();
      // depth cueing
      void DPCS();
      // depth cueing (x3)
      void DPCT();
      // interpolation of vector and far colour
      void INTPL();
      // square vector
      void SQR();
      // normal colour
      void NCS();       //TEST
      // normal colour (x3)
      void NCT();       //TEST
      // normal colour depth cue
      void NCDS();      //TEST
      // normal colour depth cue (x3)
      void NCDT();      //TEST
      // normal colour colour
      void NCCS();      //TEST
      // normal colour colour (x3)
      void NCCT();      //TEST
      // colour depth cue
      void CDP();       //TEST
      // colour colour
      void CC();
      // normal clipping
      void NCLIP();
      // average of 3 z values
      void AVSZ3();     //TEST
      // average of 4 z values
      void AVSZ4();     //TEST
      // outer product
      void OP();        //TEST
      // general purpose interpolation
      void GPF();       //TEST
      // general purpose interpolation and addition
      void GPL();       //TEST

public:

      /*** CONTROL REGS ***/
      enum
      {
            R11R12 = 0, R13R21, R22R23, R31R32, R33,
            TRX, TRY, TRZ,
            L11L12, L13L21, L22L23, L31L32, L33,
            RBK, GBK, BBK,
            LR1LR2, LR3LG1, LG2LG3, LB1LB2, LB3,
            RFC, GFC, BFC,
            OFX, OFY,
            H,
            DQA, DQB,
            ZSF3, ZSF4,
            FLAG
      };

      /*** DATA REGS ***/
      enum
      {
            VXY0 = 0, VZ0,
            VXY1, VZ1,
            VXY2, VZ2,
            RGB,
            OTZ,
            IR0, IR1, IR2, IR3,
            SXY0, SXY1, SXY2, SXYP,
            SZ0, SZ1, SZ2, SZ3,
            RGB0, RGB1, RGB2,
            RES1,
            MAC0, MAC1, MAC2, MAC3,
            IRGB, ORGB,
            LZCS, LZCR
      };

      /*** TESTING ***/
      friend test::GTEtestbench;
};

#endif
