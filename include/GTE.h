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
#include <vector>
#include <functional>

class cpu;

// gte: cop2, deals with vector and matrix transformations
class gte : public coprocessor
{
      // gte needs to access registers
      friend cpu;

      // registers. NOT general purpose
	reg32 data_reg[32];
	reg32 control_reg[32];

      // function pointers
      std::vector<std::function<void(gte*)>> instruction;

public:
	// register transfers
	void writeDataReg(word data_in, unsigned dest_reg);
	word readDataReg(unsigned source_reg);
	void writeControlReg(word data_in, unsigned dest_reg);
	word readControlReg(unsigned source_reg);
	void executeInstruction(unsigned instruction);

private:
	// instructions
      void RTPS();
      void RTPT();
      void MVMVA();
      void DPCL();
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

      // 
      enum
      {
            R11R12 = 0,
            R13R21,
            R22R23,
            R31R32,
            R33,
            TRX,
            TRY,
            TRZ,
            L11L12,
            L13L21,
            L22L23,
            L31L32,
            L33,
            RBK,
            GBK,
            BBK,
            LR1LR2,
            LR3LG1,
            LG2LG3,
            LB1LB2,
            LB3,
            RFC,
            GFC,
            BFC,
            OFX,
            OFY,
            H,
            DQA,
            DQB,
            ZSF3,
            ZSF4,
            FLAG
      };

      enum
      {
            VXY0 = 0,
            VZ0,
            VXY1,
            VZ1,
            VXY2,
            VZ2,
            RGB,
            OTZ,
            IR0,
            IR1,
            IR2,
            IR3,
            SXY0,
            SXY1,
            SXY2,
            SXYP,
            SZ0,
            SZ1,
            SZ2,
            SZ3,
            RGB0,
            RGB1,
            RGB2,
            RES1,
            MAC0,
            MAC1,
            MAC2,
            MAC3,
            IRGB,
            ORGB,
            LZCS,
            LZCR
      };
};

#endif
