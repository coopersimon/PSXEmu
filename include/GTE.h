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

// TODO: some way of stalling the cpu if it tries to execute gte command before the gte is finished with the last one

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

      //inline word read() const { return data; } // needed to remove template ambiguity
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

// note: multiplying fixed point numbers:
// multiply together, shift value to the right by 12


// gte: cop2, deals with vector and matrix transformations
class gte : public coprocessor
{
      // registers. NOT general purpose
      GTEReg control_reg[32];
	GTEDataReg data_reg[32];

      // function pointers
      std::vector<std::function<void(gte*)>> opcodes;

      // instruction
      unsigned instruction;

      // count cycles down before next gte instruction can run
      //unsigned cycle_countdown;
      // TODO: along with something like void countCycle() {cycle_countdown--;}

public:
      // TODO: constructor, register instructions
      gte();

	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg);
	word readDataReg(unsigned source_reg) const;
	void writeControlReg(word data_in, unsigned dest_reg);
	word readControlReg(unsigned source_reg) const;

      // decode and run instruction
      void executeInstruction(unsigned instruction);

private:
      // inline functions
      inline unsigned shiftFraction() const { return (instruction >> 19) & 1; }
      inline unsigned MVMVAMultiplyMatrix() const { return (instruction >> 17) & 3; }
      inline unsigned MVMVAMultiplyVector() const { return (instruction >> 15) & 3; }
      inline unsigned MVMVATranslationVector() const { return (instruction >> 13) & 3; }
      inline bool lm() const { return (instruction >> 10) & 1; }
      inline unsigned GTECommand() const { return instruction & 0x3F; }

      // Bit truncating, sets bits in FLAG register
      fixedPoint A1(const fixedPoint &input);
      fixedPoint A2(const fixedPoint &input);
      fixedPoint A3(const fixedPoint &input);
      fixedPoint B1(fixedPoint &input);
      fixedPoint B2(fixedPoint &input);
      void B3(fixedPoint &input);
      void C1(fixedPoint &input);
      void C2(fixedPoint &input);
      void C3(fixedPoint &input);
      void D(fixedPoint &input);
      void E(fixedPoint &input);
      void F(fixedPoint &input);
      void G1(fixedPoint &input);
      void G2(fixedPoint &input);
      void Hx(fixedPoint &input);


      // TODO: instructions (easy!....)
	// instructions
      void RESERVED();

      void RTPS();
      void RTPT();
      void MVMVA();
      void DCPL();
      void DPCS();
      void DPCT();
      void INTPL();
      void SQR();
      void NCS();
      void NCT();
      void NCDS();
      void NCDT();
      void NCCS();
      void NCCT();
      void CDP();
      void CC();
      void NCLIP();
      void AVSZ3();
      void AVSZ4();
      void OP();
      void GPF();
      void GPL();

public:

      // control regs
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

      // data regs
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

      // testing
      friend test::GTEtestbench;
};

#endif
